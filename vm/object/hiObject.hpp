#ifndef _HI_OBJECT_HPP
#define _HI_OBJECT_HPP

#include "object/klass.hpp"
#include <assert.h>

class HiObject {
private:
    Klass*  _klass;
    HiDict* _obj_dict;

public:
    Klass* klass()             { assert(_klass != NULL); return _klass; }
    void set_klass(Klass* x)   { _klass = x; }

    HiDict* obj_dict();
    void set_obj_dict(HiDict* d)    { _obj_dict = d; }

    void print();
    HiObject* len();
    HiObject* repr();
    HiObject* call(HiList* args, HiDict* kwargs);

    HiObject* add(HiObject* x);
    HiObject* sub(HiObject* x);
    HiObject* mul(HiObject* x);
    HiObject* div(HiObject* x);
    HiObject* mod(HiObject* x);
    HiObject* true_div(HiObject* x);

    HiObject* greater  (HiObject* x);
    HiObject* less     (HiObject* x);
    HiObject* equal    (HiObject* x);
    HiObject* not_equal(HiObject* x);
    HiObject* ge       (HiObject* x);
    HiObject* le       (HiObject* x);

    HiObject* getattr(HiObject* x);
    HiObject* setattr(HiObject* x, HiObject* y);
    HiObject* subscr(HiObject* x);

    void      store_subscr(HiObject* x, HiObject* y);
    HiObject* contains(HiObject* x);
    HiObject* iter();
    void      del_subscr(HiObject* x);

    template<typename T>
    T* as();

    void* operator new(size_t size);
};

class HiTypeObject : public HiObject {
private:
    Klass*  _own_klass;

public:
    HiTypeObject();

    void    set_own_klass(Klass* k);
    Klass*  own_klass()             { return _own_klass; }
    HiList* mro()                   { return _own_klass->mro(); }
};

class ObjectKlass : public Klass {
private:
    ObjectKlass();
    static ObjectKlass* instance;

public:
    static ObjectKlass* get_instance();
    void initialize();
};

#endif

