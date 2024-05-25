#ifndef _TYPE_OBJECT_HPP
#define _TYPE_OBJECT_HPP

#include "object/klass.hpp"
#include "object/hiObject.hpp"
#include <assert.h>

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

    virtual HiObject* getattr(HiObject* x, HiObject* y);
    virtual HiObject* setattr(HiObject* o, HiObject* x, HiObject* y);
    virtual HiObject* equal(HiObject* x, HiObject* y);
    virtual void print(HiObject* obj);

    virtual HiObject* call(HiObject* x, HiList* args, HiDict* kwargs);
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

#endif

