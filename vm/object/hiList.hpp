#ifndef _HI_LIST_HPP
#define _HI_LIST_HPP

#include "hiObject.hpp"
#include "arrayList.hpp"

class HiList : public HiObject {
private:
    ArrayList<HiObject*>* _values;

public:
    HiList();

    HiObject* get(int index);
    void append(HiObject* o);
    void set(int index, HiObject* o);
    int length();
};

#endif
