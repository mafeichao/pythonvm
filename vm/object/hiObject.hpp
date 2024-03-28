#ifndef _HI_OBJECT_HPP
#define _HI_OBJECT_HPP

#include <iostream>
using namespace std;

class HiObject {
public:
    virtual void print() {
        printf("This is an object.\n");
    }
};

#endif
