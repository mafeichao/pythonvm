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

void Klass::print(HiObject* x) {
    HiObject* func = x->getattr(ST(str));
    if (func != Universe::HiNone) {
        HiObject* s = Interpreter::get_instance()->call_virtual(func, nullptr);
        s->as<HiString>()->print();
        return;
    }

    func = x->getattr(ST(repr));
    if (func != Universe::HiNone) {
        HiObject* r = Interpreter::get_instance()->call_virtual(func, nullptr);
        r->as<HiString>()->print();
        return;
    }

    printf("<object at %p>", x);
}

HiObject* Klass::call(HiObject* x, HiList* args, HiDict* kwargs) {
    HiObject* callable = x->getattr(ST(call));

    if (callable == Universe::HiNone) {
        x->print();
        printf(" is non-callable\n");
        assert(false);
    }

    return Interpreter::get_instance()->call_virtual(callable, args);
}

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

HiObject* Klass::subscr(HiObject* x, HiObject* y) {
    HiList* args = new HiList();
    args->append(y);
    return find_and_call(x, args, ST(getitem));
}

void Klass::store_subscr(HiObject* x, HiObject* y, HiObject* z) {
    HiList* args = new HiList();
    args->append(y);
    args->append(z);
    find_and_call(x, args, ST(setitem));
}

void Klass::del_subscr(HiObject* x, HiObject* y) {
    HiList* args = new HiList();
    args->append(y);
    find_and_call(x, args, ST(delitem));
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
    HiObject* init_func = inst->getattr(ST(init));

    if (init_func != Universe::HiNone) {
        Interpreter::get_instance()->call_virtual(init_func, args);
    }

    return inst;
}

HiObject* Klass::add(HiObject* lhs, HiObject* rhs) {
    HiList* args = new HiList();
    args->append(rhs);
    return find_and_call(lhs, args, ST(add));
}

HiObject* Klass::len(HiObject* obj) {
    return find_and_call(obj, nullptr, ST(len));
}

HiObject* Klass::repr(HiObject* obj) {
    return find_and_call(obj, nullptr, ST(repr));
}

HiObject* Klass::find_and_call(HiObject* lhs, HiList* args, HiObject* func_name) {
    HiObject* func = lhs->getattr(func_name);
    if (func != Universe::HiNone) {
        return Interpreter::get_instance()->call_virtual(func, args);
    }

    printf("class ");
    lhs->klass()->name()->print();
    printf(" Error : unsupport operation for class ");
    assert(false);
    return Universe::HiNone;
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
        inst->set_own_klass(new Klass());
        if (supers->length() > 0) {
            inst->own_klass()->set_super(supers->get(0)->as<HiTypeObject>()->own_klass());
        }
        inst->own_klass()->set_klass_dict(attrs);

        return inst;
    }

    return nullptr;
}

HiTypeObject::HiTypeObject() {
    set_klass(TypeKlass::get_instance());
}

void HiTypeObject::set_own_klass(Klass* k) {
    _own_klass = k;
    k->set_type_object(this);
}

