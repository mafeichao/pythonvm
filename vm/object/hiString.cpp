#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include <string.h>

StringKlass* StringKlass::instance = nullptr;

StringKlass::StringKlass() {
}

StringKlass* StringKlass::get_instance() {
    if (instance == nullptr)
        instance = new StringKlass();

    return instance;
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

    for (int i = 0; i < str_obj->length(); i++) {
        printf("%c", str_obj->value()[i]);
    }
}


HiString::HiString(const char* x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value, x);

    set_klass(StringKlass::get_instance());
}

HiString::HiString(const char * x, const int length) {
    _length = length;
    _value = new char[length];

    // do not use strcpy here, since '\0' is allowed.
    for (int i = 0; i < length; i++) {
        _value[i] = x[i];
    }

    set_klass(StringKlass::get_instance());
}

HiObject* string_upper(ObjList args) {
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

    str_obj = new HiString(v, length);
    delete[] v;
    return str_obj;
}

