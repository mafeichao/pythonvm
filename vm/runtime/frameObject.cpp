#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/cellObject.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "memory/oopClosure.hpp"

#include <algorithm>

FrameObject::FrameObject() {
    _stack   = nullptr;
    _consts  = nullptr;
    _names   = nullptr;

    _locals  = nullptr;
    _globals = nullptr;

    _closure = nullptr;
    _fast_locals = nullptr;

    _blocks  = nullptr;

    _codes   = nullptr;
    _sender  = nullptr;

    _pc      = 0;
    _entry_frame = false;
}

FrameObject::~FrameObject() {
    _stack   = nullptr;
    _consts  = nullptr;
    _names   = nullptr;

    _locals  = nullptr;
    _globals = nullptr;

    _closure = nullptr;
    _fast_locals = nullptr;

    if (_blocks) {
        delete _blocks;
        _blocks = nullptr;
    }

    _codes   = nullptr;
    _sender  = nullptr;

    _pc      = 0;
    _entry_frame = false;
}

// this constructor is used for module only.
void FrameObject::initialize(Handle<CodeObject*> codes) {
    _codes   = codes;
    _consts  = codes->_consts;
    _names   = codes->_names;

    _stack   = HiList::new_instance();
    _locals  = HiDict::new_instance();
    _globals = _locals;
}

void FrameObject::initialize(Handle<FunctionObject*> func,
        Handle<HiList*> args, Handle<HiList*> kwargs) {
    _codes   = func->_func_code;
    _consts  = _codes->_consts;
    _names   = _codes->_names;

    _locals  = func->_locals ? func->_locals : HiDict::new_instance();
    _globals = func->_globals;
    _fast_locals = HiList::new_instance();

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
        alist = HiList::new_instance();
    }

    Handle<HiDict*> adict = nullptr;
    if (_codes->_flag & FunctionObject::CO_VARKEYWORDS) {
        adict = HiDict::new_instance();
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
        _closure = HiList::new_instance();

        for (int i = 0; i < cells->length(); i++) {
            CellObject* cell = new CellObject(nullptr);
            int index = _codes->_var_names->index(cells->get(i));
            if (index >= 0) {
                cell->set_value(_fast_locals->get(index));
            }
            _closure->append(cell);
        }
    }

    if (func->closure() && func->closure()->length() > 0) {
        if (_closure == nullptr)
            _closure = func->closure();
        else {
            _closure = _closure->add(func->closure())->as<HiList>();
        }
    }

    _stack   = HiList::new_instance();
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

BlockList* FrameObject::blocks() {
    if (!_blocks) {
        _blocks = new BlockList();
    }

    return _blocks;
}

void FrameObject::setup_block(unsigned int btype, unsigned int target, int level) {
    blocks()->add(Block(btype, target, level));
}

Block FrameObject::pop_block() {
    assert(_blocks && _blocks->length() > 0);
    return _blocks->pop();
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

HiString* FrameObject::file_name() {
    return _codes->_file_name;
}

HiString* FrameObject::func_name() {
    return _codes->_co_name;
}

int FrameObject::lineno() {
    return _pc;
}

