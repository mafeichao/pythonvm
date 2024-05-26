#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "object/typeObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"
#include "util/handles.hpp"
#include <new>
#include <cstring>

using namespace std;

StringKlass* StringKlass::instance = nullptr;

StringKlass::StringKlass() {
}

StringKlass* StringKlass::get_instance() {
    if (instance == nullptr)
        instance = new StringKlass();

    return instance;
}

void StringKlass::initialize() {
    HiDict* klass_dict = HiDict::new_instance();
    HiString* name = HiString::new_instance("upper");
    klass_dict->put(name, new FunctionObject(string_upper, name));

    set_klass_dict(klass_dict);
    (new HiTypeObject())->set_own_klass(this);
    set_name(HiString::new_instance("str"));

    add_super(ObjectKlass::get_instance());
    order_supers();
}

HiObject* StringKlass::equal(HiObject* x, HiObject* y) {
    if (x->klass() != y->klass())
        return Universe::HiFalse;

    HiString* sx = (HiString*) x;
    HiString* sy = (HiString*) y;

    assert(sx && sx->klass() == (Klass*)this);
    assert(sy && sy->klass() == (Klass*)this);

    if (sx->length() != sy->length())
        return Universe::HiFalse;

    for (int i = 0; i < sx->length(); i++) {
        if (sx->value()[i] != sy->value()[i])
            return Universe::HiFalse;
    }

    return Universe::HiTrue;
}

void StringKlass::print(HiObject* obj) {
    HiString* str_obj = (HiString*) obj;
    assert(str_obj && str_obj->klass() == (Klass*)this);

    printf("'");
    for (int i = 0; i < str_obj->length(); i++) {
        printf("%c", str_obj->value()[i]);
    }
    printf("'");
}

HiObject* StringKlass::len(HiObject* obj) {
    return new HiInteger(obj->as<HiString>()->length());
}

HiObject* StringKlass::add(HiObject* x, HiObject* y) {
    Handle<HiString*> sx = x->as<HiString>();
    Handle<HiString*> sy = y->as<HiString>();

    HiString* z = HiString::new_instance(sx->length() + sy->length());

    for (int i = 0; i < sx->length(); i++) {
        z->_value[i] = sx->_value[i];
    }

    for (int i = 0; i < sy->length(); i++) {
        z->_value[sx->length() + i] = sy->_value[i];
    }

    return z;
}

HiString::HiString() {
    set_klass(StringKlass::get_instance());
    _value = nullptr;
}

HiString* HiString::new_instance(int length) {
    Handle<HiString*> s = new HiString();
    s->_length = length;
    void* temp = Universe::heap->allocate(sizeof(char) * length);
    s->_value = new (temp) char[length];

    return s;
}

HiString* HiString::new_instance(const char* x) {
    return HiString::new_instance(x, strlen(x));
}

HiString* HiString::new_instance(const char * x, const int length) {
    Handle<HiString*> s = new HiString();
    s->_length = length;
    void* temp = Universe::heap->allocate(sizeof(char) * length);
    s->_value = new (temp) char[length];

    // do not use strcpy here, since '\0' is allowed.
    for (int i = 0; i < length; i++) {
        s->_value[i] = x[i];
    }

    return s;
}

HiObject* StringKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == (Klass*) this);
    assert(y && y->klass() == (Klass*) IntegerKlass::get_instance());

    HiString * sx = (HiString*)x;
    HiInteger* iy = (HiInteger*)y;

    return HiString::new_instance(&(sx->value()[iy->value()]), 1);
}

HiObject* StringKlass::less(HiObject* x, HiObject* y) {
    HiString* sx = (HiString*)x;
    HiString* sy = (HiString*)y;

    assert(sx && (sx->klass() == (Klass *)this));
    assert(sy && (sy->klass() == (Klass *)this));

    int len = sx->length() < sy->length() ?
        sx->length() : sy->length();

    for (int i = 0; i < len; i++) {
        if (sx->value()[i] < sy->value()[i])
            return Universe::HiTrue;
        else if (sx->value()[i] > sy->value()[i])
            return Universe::HiFalse;
    }

    if (sx->length() < sy->length()) {
        return Universe::HiTrue;
    }

    return Universe::HiFalse;
}

HiObject* string_upper(HiList* args, HiDict* kwargs) {
    HiObject* arg0 = args->get(0);
    assert(arg0->klass() == StringKlass::get_instance());

    HiString* str_obj = (HiString*)arg0;

    int length = str_obj->length();
    if (length <= 0)
        return Universe::HiNone;

    char* v = new char[length];
    char c;
    for (int i = 0; i < length; i++) {
        c = str_obj->value()[i];
        if (c >= 'a' && c <= 'z')
            v[i] = c - 0x20;
        else
            v[i] = c;
    }

    str_obj = HiString::new_instance(v, length);
    delete[] v;
    return str_obj;
}

HiObject* StringKlass::allocate_instance(HiList* args) {
    if (!args || args->length() == 0) {
        return HiString::new_instance("");
    }
    else
        return args->get(0)->as<HiString>();
}

void StringKlass::oops_do(OopClosure* closure, HiObject* obj) {
    HiString* str_obj = obj->as<HiString>();
    closure->do_raw_mem(str_obj->value_address(), str_obj->length());
}

size_t StringKlass::size() {
    return sizeof(HiString);
}

