#ifndef HI_FUNCTION_OBJECT_HPP
#define HI_FUNCTION_OBJECT_HPP

#include "code/codeObject.hpp"
#include "object/klass.hpp"

class FunctionObject;

class FunctionKlass : public Klass {
private:
    FunctionKlass();
    static FunctionKlass* instance;

public:
    static FunctionKlass* get_instance();

    virtual void print(HiObject* obj);
};

HiObject* len(HiList* args);
HiObject* object_print(HiList* args);
HiObject* isinstance(HiList* args);
HiObject* type_of(HiList* args);
HiObject* build_type_object(HiList* args);
HiObject* internal_exec(FunctionObject* callable, HiDict* globals, HiDict* locals);

typedef HiObject* (*NativeFuncPointer)(HiList* args);

class FunctionObject : public HiObject {
friend class FunctionKlass;
friend class FrameObject;

private:
    CodeObject* _func_code;
    HiString*   _func_name;
    HiDict*     _globals;
    HiDict*     _locals;
    HiList*     _defaults;
    HiList*     _closure;

    NativeFuncPointer _native_func;

    unsigned int _flags;

public:
    enum CO_FLAGS {
        CO_VARARGS = 0x4,
        CO_VARKEYWORDS = 0x8,
        CO_GENERATOR = 0x20,
    };

    FunctionObject(HiObject* code_object);

    FunctionObject(NativeFuncPointer nfp);

    FunctionObject(Klass* klass) {
        _func_code = nullptr;
        _func_name = nullptr;
        _flags     = 0;
        _globals   = nullptr;
        _locals    = nullptr;
        _defaults  = nullptr;

        set_klass(klass);
    }
    
    HiString*  func_name()   { return _func_name; }
    int  flags()             { return _flags; }

    HiDict*    globals()        { return _globals; }
    void set_globals(HiDict* x) { _globals = x; }
    HiDict*    locals()         { return _locals; }
    void set_locals(HiDict* x)  { _locals = x; }

    void set_default(HiList* defaults);
    HiList* defaults()       { return _defaults; }

    void set_closure(HiList* x) { _closure = x; }
    HiList* closure()        { return _closure; }

    HiObject*  call(HiList* args);
};

// Method objects.
class MethodKlass : public Klass {
private:
    MethodKlass();
    static MethodKlass* instance;

public:
    static MethodKlass* get_instance();
};

class MethodObject : public HiObject {
friend class MethodKlass;

private:
    HiObject* _owner;
    FunctionObject* _func;

public:
    MethodObject(FunctionObject* func) : _owner(NULL), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    MethodObject(FunctionObject* func, HiObject* owner) : _owner(owner), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    void set_owner(HiObject * x)   { _owner = x; }
    HiObject* owner()              { return _owner; }
    FunctionObject* func()         { return _func; }

    static bool is_native(HiObject* x);
    static bool is_method(HiObject* x);
    static bool is_function(HiObject* x);
};

class NativeFunctionKlass : public Klass {
private:
    NativeFunctionKlass();
    static NativeFunctionKlass* instance;

public:
    static NativeFunctionKlass* get_instance();
};

#endif

