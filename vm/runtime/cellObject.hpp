#ifndef CELL_OBJECT_HPP
#define CELL_OBJECT_HPP

#include "object/klass.hpp"
#include "object/hiObject.hpp"
#include "memory/oopClosure.hpp"

class HiList;

class CellKlass : public Klass {
private:
    CellKlass();
    static CellKlass* _instance;

public:
    static CellKlass* get_instance();

    virtual size_t size();
    virtual void oops_do(OopClosure* f, HiObject* obj);
};

class CellObject : public HiObject {
friend class CellKlass;

private:
    HiObject* _value;

public:
    CellObject(HiObject*);
    HiObject* value()                           { return _value; }
    void set_value(HiObject* o)                 { _value = o; }
};

#endif

