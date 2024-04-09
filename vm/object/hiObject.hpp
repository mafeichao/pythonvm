#ifndef _HI_OBJECT_HPP
#define _HI_OBJECT_HPP

#include <iostream>
using namespace std;

class HiObject {
public:
    virtual void print() {
        printf("This is an object.\n");
    }

    virtual HiObject* add      (HiObject* x) { return nullptr; }

    virtual HiObject* greater  (HiObject* x) { return nullptr; }
    virtual HiObject* less     (HiObject* x) { return nullptr; }
    virtual HiObject* equal    (HiObject* x) { return nullptr; }
    virtual HiObject* not_equal(HiObject* x) { return nullptr; }
    virtual HiObject* ge       (HiObject* x) { return nullptr; }
    virtual HiObject* le       (HiObject* x) { return nullptr; }
};

#endif
