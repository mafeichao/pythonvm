#include "object/klass.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiString.hpp"

// getattr for normal object.
// a = A()
// a.b = 1
HiObject* Klass::getattr(HiObject* x, HiObject* y) {
    if (x->obj_dict()->has_key(y)) {
        return x->obj_dict()->get(y);
    }

    HiObject* result = _klass_dict->get(y);
    // Only klass attribute needs bind.
    if (!MethodObject::is_method(result) &&
        MethodObject::is_function(result)) {
        result = new MethodObject(result->as<FunctionObject>(), x);
    }

    return result;
}

// setattr for normal object.
HiObject* Klass::setattr(HiObject* obj, HiObject* x, HiObject* y) {
    obj->obj_dict()->put(x, y);
    return Universe::HiNone;
}

HiObject* Klass::create_klass(HiDict* klass_dict, HiList* supers_list, HiString* name) {
    Klass* new_klass   = new Klass();

    new_klass->set_klass_dict(klass_dict);
    new_klass->set_name((HiString*)name);

    if (supers_list->length() > 0) {
        HiTypeObject* super = supers_list->get(0)->as<HiTypeObject>();
        new_klass->set_super(super->own_klass());
    }

    HiTypeObject* type_obj = new HiTypeObject();
    type_obj->set_own_klass(new_klass);
    
    return type_obj;
}

HiObject* Klass::allocate_instance(HiList* args) {
    HiObject* inst = new HiObject();
    inst->set_klass(this);
    HiObject* constructor = inst->getattr(ST(init));
    if (constructor != Universe::HiNone) {
        Interpreter::get_instance()->call_virtual(constructor, args);
    }
    return inst;
}

/*
 * TypeObject is a special object
 */
TypeKlass* TypeKlass::instance = NULL;

TypeKlass* TypeKlass::get_instance() {
    if (instance == NULL)
        instance = new TypeKlass();

    return instance;
}

void TypeKlass::initialize() {
    HiTypeObject* tp_obj = new HiTypeObject();
    set_name(new HiString("type"));
    tp_obj->set_own_klass(this);
    set_super(ObjectKlass::get_instance());
}

void TypeKlass::print(HiObject* obj) {
    Klass* own_klass = obj->as<HiTypeObject>()->own_klass();
    printf("<class ");
    own_klass->name()->print();
    printf(">");
}

HiObject* TypeKlass::getattr(HiObject* x, HiObject* y) {
    return x->as<HiTypeObject>()->own_klass()->klass_dict()->get(y);
}

HiObject* TypeKlass::setattr(HiObject* obj, HiObject* x, HiObject* y) {
    obj->as<HiTypeObject>()->own_klass()->klass_dict()->put(x, y);
    return Universe::HiNone;
}

HiTypeObject::HiTypeObject() {
    set_klass(TypeKlass::get_instance());
}

void HiTypeObject::set_own_klass(Klass* k) {
    _own_klass = k;
    k->set_type_object(this);
}

