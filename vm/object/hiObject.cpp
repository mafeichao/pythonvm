#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/typeObject.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"
#include "runtime/cellObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/module.hpp"
#include "runtime/traceback.hpp"
#include "util/handles.hpp"
#include <cstdint>

using namespace std;

HiDict* HiObject::obj_dict() {
    if (!_obj_dict) {
        _obj_dict = HiDict::new_instance();
    }

    return _obj_dict;
}

void HiObject::print() {
    klass()->print(this);
}

HiObject* HiObject::len() {
    return klass()->len(this);
}

HiObject* HiObject::repr() {
    return klass()->repr(this);
}

HiObject* HiObject::call(HiList* args, HiDict* kwargs) {
    return klass()->call(this, args, kwargs);
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
    return klass()->getattr(this, x);
}

HiObject* HiObject::setattr(HiObject* x, HiObject* y) {
    return klass()->setattr(this, x, y);
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

/*
 * Interfaces for GC.
 */
void HiObject::oops_do(OopClosure* closure) {
    // object does not know who to visit, klass knows
    closure->do_oop((HiObject**)&_obj_dict);
    klass()->oops_do(closure, this);
}

char* HiObject::new_address() {
    if ((_mark_word & 0x1) == 0x1)
        return (char*)(_mark_word & ((uintptr_t)-8));

    return NULL;
}

void HiObject::set_new_address(char* addr) {
    if (!addr)
        return;

    _mark_word = ((uintptr_t)addr) | 0x1;
}

size_t HiObject::size() {
    return klass()->size();
}

void* HiObject::operator new(size_t size) {
    return Universe::heap->allocate(size);
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
ModuleObject* HiObject::as<ModuleObject>() {
    assert(this->klass() == ModuleKlass::get_instance());
    return (ModuleObject*)this;
}

template<>
ListIterator* HiObject::as<ListIterator>() {
    assert(klass() == ListIteratorKlass::get_instance());
    return (ListIterator*)this;
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
Traceback* HiObject::as<Traceback>() {
    assert(this->klass() == TracebackKlass::get_instance());
    return (Traceback*)this;
}

template<>
HiTypeObject* HiObject::as<HiTypeObject>() {
    assert(this->klass() == TypeKlass::get_instance());
    return (HiTypeObject*)this;
}

template<>
FunctionObject* HiObject::as<FunctionObject>() {
    assert(this->klass() == FunctionKlass::get_instance() ||
        klass() == NativeFunctionKlass::get_instance());
    return (FunctionObject*)this;
}

template<>
MethodObject* HiObject::as<MethodObject>() {
    assert(klass() == MethodKlass::get_instance());
    return (MethodObject*)this;
}

/*
 * Klass for object.
 */
ObjectKlass* ObjectKlass::instance = NULL;

ObjectKlass::ObjectKlass() {
}

void ObjectKlass::initialize() {
    set_name(HiString::new_instance("object"));
    (new HiTypeObject())->set_own_klass(this);
    set_super_list(nullptr);
    set_klass_dict(HiDict::new_instance());

    order_supers();
}

ObjectKlass* ObjectKlass::get_instance() {
    if (instance == NULL)
        instance = new ObjectKlass();

    return instance;
}

