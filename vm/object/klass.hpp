#ifndef _KLASS_HPP
#define _KLASS_HPP

#include <stdio.h>
#include "object/arrayList.hpp"

class HiObject;
class HiString;
class HiDict;
class HiTypeObject;

class Klass {
private:
    Klass*        _super;
    HiTypeObject* _type_object;
    HiString*     _name;
    HiDict*       _klass_dict;

public:
    Klass() {};

    void set_super(Klass* x)              { _super = x; }
    Klass* super()                        { return _super; }

    void set_type_object(HiTypeObject* x) { _type_object = x; }
    HiTypeObject* type_object()           { return _type_object; }

    void set_name(HiString* x)            { _name = x; }
    HiString* name()                      { return _name; }

    void set_klass_dict(HiDict* dict)     { _klass_dict = dict; }
    HiDict* klass_dict()                  { return _klass_dict; }

    virtual void print(HiObject* obj) {};

    virtual HiObject* greater  (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* less     (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* equal    (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* not_equal(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* ge       (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* le       (HiObject* x, HiObject* y) { return nullptr; }

    virtual HiObject* add(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* sub(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* mul(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* div(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* mod(HiObject* x, HiObject* y) { return nullptr; }

    virtual HiObject* call(ArrayList<HiObject*>* args) { return nullptr; }
    virtual HiObject* iter(HiObject* x)                { return nullptr; }

    virtual HiObject* subscr (HiObject* x, HiObject* y)                 { return nullptr; }
    virtual void store_subscr  (HiObject* x, HiObject* y, HiObject* z)  { return; }
    virtual void del_subscr    (HiObject* x, HiObject* y)               { return; }
    virtual HiObject* contains (HiObject* x, HiObject* y)               { return nullptr; }
};

/*
 * meta-klass for the object system.
 */
class TypeKlass : public Klass {
private:
    TypeKlass() {}
    static TypeKlass* instance;

public:
    static TypeKlass* get_instance();
    void initialize();

    virtual void print(HiObject* obj);
};

#endif

