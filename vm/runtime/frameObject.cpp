#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"

// this constructor is used for module only.
FrameObject::FrameObject(CodeObject* codes) {
    _codes   = codes;
    _consts  = codes->_consts;
    _names   = codes->_names;
    
    _locals  = new HiDict();
    _globals = _locals;
    _fast_locals = nullptr; // useless

    _stack   = new HiList();

    _pc      = 0;
    _sender  = NULL;
}

FrameObject::FrameObject (FunctionObject* func, ObjList args, HiList* kwargs) {
    _codes   = func->_func_code;
    _consts  = _codes->_consts;
    _names   = _codes->_names;

    _locals  = new HiDict();
    _globals = func->_globals;
    _fast_locals = new ArrayList<HiObject*>();

    const int argcnt  = _codes->_argcount;
    const int na = args == nullptr ? 0 : args->length();
    const int nk = kwargs == nullptr ? 0 : kwargs->size();
    int kw_pos = argcnt;

    if (na < argcnt) {
        _codes->_co_name->print();
        printf(" missing %d required positional argument\n", argcnt - na);
        assert(false);
    }

    if (func->_defaults) {
        int dft_cnt = func->_defaults->length();
        int argnum  = _codes->_argcount;

        while (dft_cnt--) {
            _fast_locals->set(--argnum, func->_defaults->get(dft_cnt));
        }
    }

    HiList* alist = nullptr;
    if (_codes->_flag & FunctionObject::CO_VARARGS) {
        alist = new HiList();
    }

    HiDict* adict = nullptr;
    if (_codes->_flag & FunctionObject::CO_VARKEYWORDS) {
        adict = new HiDict();
    }

    for (int i = 0; i < argcnt; i++) {
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
            HiObject* key = kwargs->get(i);
            HiObject* value = args->get(na - nk + i);

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
        _fast_locals->add(alist);
    }

    if (_codes->_flag & FunctionObject::CO_VARKEYWORDS) {
        _fast_locals->add(adict);
    }

    _stack   = new HiList();

    _pc      = 0;
    _sender  = NULL;
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
