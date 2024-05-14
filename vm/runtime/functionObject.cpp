#include "object/klass.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

FunctionKlass* FunctionKlass::instance = nullptr;

FunctionKlass* FunctionKlass::get_instance() {
    if (instance == nullptr)
        instance = new FunctionKlass();

    return instance;
}

FunctionKlass::FunctionKlass() {
}

void FunctionKlass::print(HiObject* obj) {
    printf("<function : ");
    FunctionObject* fo = static_cast<FunctionObject*>(obj);

    assert(fo && fo->klass() == (Klass*) this);
    fo->func_name()->print();
    printf(">");
}

FunctionObject::FunctionObject(HiObject* code_object) {
    CodeObject* co = (CodeObject*) code_object;

    _func_code = co;
    _func_name = co->_co_name;
    _flags     = co->_flag;
    _globals   = nullptr;
    _closure   = nullptr;

    set_klass(FunctionKlass::get_instance());
}

FunctionObject::FunctionObject(NativeFuncPointer nfp) {
    _func_code = nullptr;
    _func_name = nullptr;
    _flags     = 0;
    _globals   = nullptr;
    _closure   = nullptr;
    _native_func = nfp;

    set_klass(NativeFunctionKlass::get_instance());
}

void FunctionObject::set_default(HiList* defaults) {
    if (defaults == nullptr) {
        _defaults = nullptr;
        return;
    }

    _defaults = new HiList();

    for (int i = 0; i < defaults->length(); i++) {
        _defaults->set(i, defaults->get(i));
    }
}

/*
 * Operations for native calls.
 */
NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
    if (instance == nullptr)
        instance = new NativeFunctionKlass();

    return instance;
}

NativeFunctionKlass::NativeFunctionKlass() {
    set_super(FunctionKlass::get_instance());
}

HiObject* FunctionObject::call(HiList* args) {
    return (*_native_func)(args);
}

/*
 *  Operations for methods
 *  Method is a wrapper for function.
 */
MethodKlass* MethodKlass::instance = NULL;

MethodKlass* MethodKlass::get_instance() {
    if (instance == NULL)
        instance = new MethodKlass();

    return instance;
}

MethodKlass::MethodKlass() {
    set_klass_dict(new HiDict());
    set_super(FunctionKlass::get_instance());
}

/*
 * To check the type of a callable object.
 */
bool MethodObject::is_native(HiObject *x) {
    Klass* k = x->klass();
    if (k == (Klass*) NativeFunctionKlass::get_instance())
        return true;

    while (k->super() != NULL) {
        k = k->super();
        if (k == (Klass*) NativeFunctionKlass::get_instance())
            return true;
    }
    return false;
}

bool MethodObject::is_method(HiObject *x) {
    if (x->klass() == (Klass*) MethodKlass::get_instance())
        return true;

    return false;
}

bool MethodObject::is_function(HiObject *x) {
    Klass* k = x->klass();
    if (k == (Klass*) FunctionKlass::get_instance())
        return true;

    while (k->super() != NULL) {
        k = k->super();
        if (k == (Klass*) FunctionKlass::get_instance())
            return true;
    }

    return false;
}

HiObject* len(HiList* args) {
    return new HiInteger(args->get(0)->as<HiString>()->length());
}

HiObject* object_print(HiList* args) {
    HiObject* arg0 = args->get(0);
    arg0->print();
    printf("\n");

    return Universe::HiNone;
}

HiObject* isinstance(HiList* args) {
    HiObject* x = args->get(0);
    HiTypeObject* y = args->get(1)->as<HiTypeObject>();

    Klass* k = x->klass();
    while (k != nullptr) {
        if (k == y->own_klass())
            return Universe::HiTrue;

        k = k->super();
    }

    return Universe::HiFalse;
}

HiObject* type_of(HiList* args) {
    HiObject* arg0 = args->get(0);
    return arg0->klass()->type_object();
}

HiObject* build_type_object(HiList* args) {
    int length = args->length();
    assert(length >= 2);
    FunctionObject* cls_def = args->get(0)->as<FunctionObject>();
    HiString* name = args->get(1)->as<HiString>();
    HiList* super_list = new HiList();

    for (int i = 2; i < length; i++) {
        super_list->append(args->get(i));
    }

    HiDict* locals = new HiDict();
    internal_exec(cls_def, nullptr, locals);
    return Klass::create_klass(locals, super_list, name);
}

HiObject* internal_exec(FunctionObject* callable, HiDict* globals, HiDict* locals) {
    if (globals) {
        callable->set_globals(globals);
    }

    if (locals) {
        callable->set_locals(locals);
    }

    return Interpreter::get_instance()->call_virtual(callable, nullptr);
}

