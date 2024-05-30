#include "object/klass.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "object/typeObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "memory/oopClosure.hpp"

#include <stdio.h>

IntegerKlass* IntegerKlass::instance = NULL;

IntegerKlass::IntegerKlass() {
}

void IntegerKlass::initialize() {
    set_klass_dict(HiDict::new_instance());
    set_name(HiString::new_instance("int"));
    (new HiTypeObject())->set_own_klass(this);

    add_super(ObjectKlass::get_instance());
    order_supers();
}

HiInteger::HiInteger(int x) {
    _value = x;
    set_klass(IntegerKlass::get_instance());
}

IntegerKlass* IntegerKlass::get_instance() {
    if (instance == NULL)
        instance = new IntegerKlass();

    return instance;
}

void IntegerKlass::print(HiObject* obj) {
    printf("%d", obj->as<HiInteger>()->value());
}


HiObject* IntegerKlass::greater(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    if (ix > iy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* IntegerKlass::less(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    if (ix < iy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* IntegerKlass::equal(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    if (ix == iy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* IntegerKlass::not_equal(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    if (ix != iy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* IntegerKlass::ge(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    if (ix >= iy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* IntegerKlass::le(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    if (ix <= iy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* IntegerKlass::add(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    return new HiInteger(ix + iy);
}

HiObject* IntegerKlass::sub(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    return new HiInteger(ix - iy);
}

HiObject* IntegerKlass::mul(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    return new HiInteger(ix * iy);
}

HiObject* IntegerKlass::div(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();

    if (iy == 0) {
        Interpreter::get_instance()->raise_error("ZeroDivisionError");
        return nullptr;
    }

    return new HiInteger(ix / iy);
}

HiObject* IntegerKlass::mod(HiObject* x, HiObject* y) {
    int ix = x->as<HiInteger>()->value();
    int iy = y->as<HiInteger>()->value();
    assert(iy != 0);

    return new HiInteger(ix % iy);
}

HiObject* IntegerKlass::allocate_instance(HiList* args) {
    if (!args || args->length() == 0)
        return new HiInteger(0);
    else
        return nullptr;
}

size_t IntegerKlass::size() {
    return sizeof(HiInteger);
}

void IntegerKlass::oops_do(OopClosure* f, HiObject* obj) {
    // do nothing
    // only overwrite warning message in Klass
}

