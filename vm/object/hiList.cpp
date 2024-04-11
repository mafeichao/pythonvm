#include "object/hiList.hpp"

HiList::HiList() {
    _values = new ArrayList<HiObject*>();
}

HiObject* HiList::get(int index) {
    return _values->get(index);
}

void HiList::set(int index, HiObject* o) {
    return _values->set(index, o);
}

void HiList::append(HiObject* o) {
    return _values->add(o);
}

int HiList::length() {
    return _values->length();
}
