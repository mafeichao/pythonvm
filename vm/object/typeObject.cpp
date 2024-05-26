#include "object/hiDict.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/typeObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/stringTable.hpp"
#include "memory/oopClosure.hpp"

/*
 * TypeObject is a special object
 */
TypeKlass* TypeKlass::instance = nullptr;

TypeKlass* TypeKlass::get_instance() {
    if (instance == nullptr)
        instance = new TypeKlass();

    return instance;
}

void TypeKlass::initialize() {
    HiTypeObject* tp_obj = new HiTypeObject();
    set_name(HiString::new_instance("type"));
    tp_obj->set_own_klass(this);
    set_klass_dict(HiDict::new_instance());

    add_super(ObjectKlass::get_instance());
    order_supers();
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

HiObject* TypeKlass::equal(HiObject* x, HiObject* y) {
    if (x->as<HiTypeObject>() == y->as<HiTypeObject>()) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* TypeKlass::call(HiObject* x, HiList* args, HiDict* kwargs) {
    // The type object.
    HiTypeObject* to = x->as<HiTypeObject>();
    if (to->klass() != to->own_klass()) {
        return to->own_klass()->allocate_instance(args);
    }

    if (args->length() == 1) {
        return type_of(args, nullptr);
    }
    else if (args->length() == 3) {
        HiString* name = args->get(0)->as<HiString>();
        HiList* supers = args->get(1)->as<HiList>();
        HiDict* attrs  = args->get(2)->as<HiDict>();

        HiTypeObject* inst = new HiTypeObject();
        inst->set_klass(this);
        Klass* cls_klass = new Klass();
        inst->set_own_klass(cls_klass);

        cls_klass->set_name(name);
        cls_klass->set_klass_dict(attrs);
        if (supers->length() == 0) {
            supers->append(ObjectKlass::get_instance()->type_object());
        }
        cls_klass->set_super_list(supers);
        cls_klass->order_supers();

        return inst;
    }

    return nullptr;
}

void TypeKlass::oops_do(OopClosure* f, HiObject* obj) {
}

size_t TypeKlass::size() {
    return sizeof(HiTypeObject);
}

// Methods of HiTypeObject.
HiTypeObject::HiTypeObject() {
    set_klass(TypeKlass::get_instance());
}

void HiTypeObject::set_own_klass(Klass* k) {
    _own_klass = k;
    k->set_type_object(this);
}

