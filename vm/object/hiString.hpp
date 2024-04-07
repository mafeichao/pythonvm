#ifndef _HI_STRING_HPP
#define _HI_STRING_HPP

#include "hiObject.hpp"

class HiString : public HiObject {
private:
    char* _value;
    int   _length;

public:
    HiString(const char * x);
    HiString(const char * x, const int length);

    const char* value()     { return _value; }
    int length()            { return _length; }

    virtual void print() {
        for (int i = 0; i < _length; i++) {
            putchar(_value[i]);
        }
    }
};

#endif
