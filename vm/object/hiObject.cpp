#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "runtime/cellObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

void HiObject::print() {
    klass()->print(this);
}

HiObject* HiObject::greater(HiObject * rhs) {
    return klass()->greater(this, rhs);
}

HiObject* HiObject::add(HiObject * rhs) {
    return klass()->add(this, rhs);
}

HiObject* HiObject::sub(HiObject * rhs) {
    return klass()->sub(this, rhs);
}

HiObject* HiObject::mul(HiObject * rhs) {
    return klass()->mul(this, rhs);
}

HiObject* HiObject::div(HiObject * rhs) {
    return klass()->div(this, rhs);
}

HiObject* HiObject::mod(HiObject * rhs) {
    return klass()->mod(this, rhs);
}

HiObject* HiObject::true_div(HiObject * rhs) {
    return klass()->div(this, rhs);
}

HiObject* HiObject::less(HiObject * rhs) {
    return klass()->less(this, rhs);
}

HiObject* HiObject::equal(HiObject * rhs) {
    return klass()->equal(this, rhs);
}

HiObject* HiObject::not_equal(HiObject * rhs) {
    return klass()->not_equal(this, rhs);
}

HiObject* HiObject::ge(HiObject * rhs) {
    return klass()->ge(this, rhs);
}

HiObject* HiObject::le(HiObject * rhs) {
    return klass()->le(this, rhs);
}

HiObject* HiObject::getattr(HiObject* x) {
    HiObject* result = Universe::HiNone;

    result = klass()->getattr(this, x);

    // Only klass attribute needs bind.
    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject*)result, this);
    }
    return result;
}

HiObject* HiObject::subscr(HiObject* x) {
    return klass()->subscr(this, x);
}

void HiObject::store_subscr(HiObject* x, HiObject* y) {
    klass()->store_subscr(this, x, y);
}

void HiObject::del_subscr(HiObject* x) {
    klass()->del_subscr(this, x);
}

HiObject* HiObject::contains(HiObject* x) {
    return klass()->contains(this, x);
}

HiObject* HiObject::iter() {
    return klass()->iter(this);
}

template<typename T>
T* HiObject::as() {
   return (T*)this;
}

template<>
HiList* HiObject::as<HiList>() {
    assert(this->klass() == ListKlass::get_instance());
    return (HiList*)this;
}

template<>
HiDict* HiObject::as<HiDict>() {
    assert(this->klass() == DictKlass::get_instance());
    return (HiDict*)this;
}

template<>
HiString* HiObject::as<HiString>() {
    assert(this->klass() == StringKlass::get_instance());
    return (HiString*)this;
}

template<>
CellObject* HiObject::as<CellObject>() {
    assert(this->klass() == CellKlass::get_instance());
    return (CellObject*)this;
}

template<>
HiInteger* HiObject::as<HiInteger>() {
    assert(this->klass() == IntegerKlass::get_instance());
    return (HiInteger*)this;
}

template<>
HiTypeObject* HiObject::as<HiTypeObject>() {
    assert(this->klass() == TypeKlass::get_instance());
    return (HiTypeObject*)this;
}

template<>
FunctionObject* HiObject::as<FunctionObject>() {
    assert(this->klass() == FunctionKlass::get_instance());
    return (FunctionObject*)this;
}

/*
 * Klass for object.
 */
ObjectKlass* ObjectKlass::instance = NULL;

ObjectKlass::ObjectKlass() {
}

void ObjectKlass::initialize() {
    set_name(new HiString("object"));
    (new HiTypeObject())->set_own_klass(this);
    set_super(nullptr);
    set_klass_dict(new HiDict());
}

ObjectKlass* ObjectKlass::get_instance() {
    if (instance == NULL)
        instance = new ObjectKlass();

    return instance;
}

