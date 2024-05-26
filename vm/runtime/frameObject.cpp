#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "memory/oopClosure.hpp"

#include <algorithm>

// this constructor is used for module only.
FrameObject::FrameObject(Handle<CodeObject*> codes) {
    Handle<FrameObject*>(this);
    _codes   = codes;
    _consts  = codes->_consts;
    _names   = codes->_names;
    _globals = nullptr;
    _fast_locals = nullptr; // useless

    _pc      = 0;
    _sender  = NULL;
    _entry_frame = true;

    _stack   = new HiList();
    _locals  = new HiDict();
    _globals = _locals;
}

FrameObject::FrameObject(Handle<FunctionObject*> func,
        Handle<HiList*> args, Handle<HiList*> kwargs) {
    Handle<FrameObject*> temp(this);
    _codes   = func->_func_code;
    _consts  = _codes->_consts;
    _names   = _codes->_names;

    _locals  = func->_locals ? func->_locals : new HiDict();
    _globals = func->_globals;
    _closure = nullptr;

    _fast_locals = new HiList();

    const int argcnt  = _codes->_argcount;
    const int na = args == nullptr ? 0 : args->length();
    const int nk = kwargs == nullptr ? 0 : kwargs->length();
    int dft_cnt = func->_defaults == nullptr ? 0 : func->_defaults->length();

    if (na + dft_cnt < argcnt) {
        _codes->_co_name->print();
        printf(" missing %d required positional argument\n", argcnt - na);
        assert(false);
    }

    if (func->_defaults) {
        int argnum  = _codes->_argcount;

        while (dft_cnt--) {
            _fast_locals->set(--argnum, func->_defaults->get(dft_cnt));
        }
    }

    Handle<HiList*> alist = nullptr;
    if (_codes->_flag & FunctionObject::CO_VARARGS) {
        alist = new HiList();
    }

    Handle<HiDict*> adict = nullptr;
    if (_codes->_flag & FunctionObject::CO_VARKEYWORDS) {
        adict = new HiDict();
    }

    for (int i = 0; i < std::min(argcnt, na - nk); i++) {
        _fast_locals->set(i, args->get(i));
    }

    if (argcnt < na - nk) {
        if (_codes->_flag & FunctionObject::CO_VARARGS) {
            for (int i = argcnt; i < na - nk; i++) {
                alist->append(args->get(i));
            }
        }
        else {
            report_error("got an unexpected keyword argument", 
                _codes->_co_name, args->get(argcnt));
        }
    }

    if (nk > 0) {
        for (int i = 0; i < nk; i++) {
            Handle<HiObject*> key = kwargs->get(i);
            Handle<HiObject*> value = args->get(na - nk + i);

            int index = _codes->_var_names->index(key);

            if (index < 0 || index >= argcnt) {
                if (_codes->_flag & FunctionObject::CO_VARKEYWORDS) {
                    adict->put(key, value);
                }
                else {
                    report_error("got an unexpected keyword argument", 
                        _codes->_co_name, key);
                }
                continue;
            }

            if (index < na - nk) {
                report_error("got multiple values for argument", _codes->_co_name, key);
            }

            _fast_locals->set(index, value);
        }
    }

    if (_codes->_flag & FunctionObject::CO_VARARGS) {
        _fast_locals->append(alist);
    }

    if (_codes->_flag & FunctionObject::CO_VARKEYWORDS) {
        _fast_locals->append(adict);
    }

    // 处理闭包中的cell vars
    Handle<HiList*> cells = _codes->_cell_vars;
    if (cells && cells->length() > 0) {
        _closure = new HiList();

        for (int i = 0; i < cells->length(); i++) {
            _closure->append(nullptr);
        }
    }

    if (func->closure() && func->closure()->length() > 0) {
        if (_closure == nullptr)
            _closure = func->closure();
        else {
            _closure = _closure->add(func->closure())->as<HiList>();
        }
    }

    _stack   = new HiList();

    _pc      = 0;
    _sender  = NULL;
    _entry_frame = false;
}

void FrameObject::report_error(const char* msg, HiObject* func_name, HiObject* arg_name) {
    func_name->print();
    printf("() ");
    printf("%s", msg);
    printf("'");
    arg_name->print();
    printf("'\n");
    assert(false);
}

int FrameObject::get_op_arg() {
    return  _codes->_bytecodes->value()[_pc++] & 0xff;
}

unsigned char FrameObject::get_op_code() {
    return _codes->_bytecodes->value()[_pc++];
}

bool FrameObject::has_more_codes() {
    return _pc < _codes->_bytecodes->length();
}

HiObject* FrameObject::get_cell_from_parameter(int i) {
    Handle<HiObject*> cell_name = _codes->_cell_vars->get(i);
    i = _codes->_var_names->index(cell_name);
    return _fast_locals->get(i);
}

void FrameObject::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&_stack);
    f->do_oop((HiObject**)&_consts);
    f->do_oop((HiObject**)&_names);

    f->do_oop((HiObject**)&_locals);
    f->do_oop((HiObject**)&_globals);
    f->do_oop((HiObject**)&_closure);
    f->do_oop((HiObject**)&_fast_locals);

    f->do_oop((HiObject**)&_codes);

    if (_sender)
        _sender->oops_do(f);
}

