#ifndef _KLASS_HPP
#define _KLASS_HPP

#include <stdio.h>
#include "util/map.hpp"
#include "object/arrayList.hpp"

class HiObject;
class HiString;

class Klass {
private:
    Klass*        _super;
    HiString*     _name;

public:
    Klass() {};

    void set_super(Klass* x)              { _super = x; }
    Klass* super()                        { return _super; }

    void set_name(HiString* x)            { _name = x; }
    HiString* name()                      { return _name; }

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
};

#endif

