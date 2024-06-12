#include "object/klass.hpp"
#include "object/hiDict.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiDouble.hpp"
#include "object/typeObject.hpp"
#include "runtime/universe.hpp"

#include <stdio.h>

DoubleKlass* DoubleKlass::instance = NULL;

DoubleKlass::DoubleKlass() {
}

void DoubleKlass::initialize() {
    set_klass_dict(HiDict::new_instance());
    set_name(HiString::new_instance("double"));
    (new HiTypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
    order_supers();
}

HiDouble::HiDouble(double x) {
    _value = x;
    set_klass(DoubleKlass::get_instance());
}

DoubleKlass* DoubleKlass::get_instance() {
    if (instance == NULL)
        instance = new DoubleKlass();

    return instance;
}

void DoubleKlass::print(HiObject* obj) {
    printf("%.12g", obj->as<HiDouble>()->value());
}

HiObject* DoubleKlass::allocate_instance(HiObject* callable, 
        ArrayList<HiObject*>* args) {
    if (!args || args->length() == 0)
        return new HiDouble(0);
    else
        return NULL;
}

size_t DoubleKlass::size() {
    return sizeof(HiDouble);
}

void DoubleKlass::oops_do(OopClosure* f, HiObject* obj) {
    // do nothing
    // only overwrite warning message in Klass
}

HiObject* DoubleKlass::greater(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    if (dx > dy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* DoubleKlass::less(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    if (dx < dy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* DoubleKlass::equal(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    if (dx == dy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* DoubleKlass::not_equal(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    if (dx != dy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* DoubleKlass::ge(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    if (dx >= dy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* DoubleKlass::le(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    if (dx <= dy)
        return Universe::HiTrue;
    else
        return Universe::HiFalse;
}

HiObject* DoubleKlass::add(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    return new HiDouble(dx + dy);
}

HiObject* DoubleKlass::sub(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    return new HiDouble(dx - dy);
}

HiObject* DoubleKlass::mul(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    return new HiDouble(dx * dy);
}

HiObject* DoubleKlass::div(HiObject* x, HiObject* y) {
    double dx = x->as<HiDouble>()->value();
    double dy = y->as<HiDouble>()->value();

    return new HiDouble(dx / dy);

    return NULL;
}

HiObject* DoubleKlass::mod(HiObject* x, HiObject* y) {
    return new HiDouble(0.0);
}
