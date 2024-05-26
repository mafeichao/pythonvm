#ifndef CODE_OBJECT_HPP
#define CODE_OBJECT_HPP

#include "object/hiObject.hpp"

class HiString;
class HiList;

template <typename T>
class ArrayList;

class CodeKlass : public Klass {
private:
    CodeKlass();
    static CodeKlass* instance;

public:
    static CodeKlass* get_instance();

    virtual void oops_do(OopClosure* closure, HiObject* obj);
    virtual size_t size();
};


class CodeObject : public HiObject {
public:
    int _argcount;
    int _posonlyargcount;
    int _kwonlyargcount;
    int _nlocals;
    int _stack_size;
    int _flag;

    HiString* _bytecodes;
    HiList*  _names;
    HiList*  _consts;
    HiList*  _var_names;

    HiList*  _free_vars;
    HiList*  _cell_vars;

    HiString* _co_name;
    HiString* _file_name;

    int _lineno;
    HiString* _notable;

    CodeObject(int argcount, int posonlyargcount, int kwonlyargcount, int nlocals, int stacksize, 
        int flag, HiString* bytecodes, HiList* consts, 
        HiList* names, HiList* varnames, HiList* freevars, HiList* cellvars,
        HiString* file_name, HiString* co_name, int lineno, HiString* notable);
};

#endif
