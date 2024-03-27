#include "object/hiList.hpp"

HiList::HiList() {
    _values = new ArrayList<HiObject*>();
}

HiObject* HiList::get(int index) {
    return _values->get(index);
}

void HiList::add(HiObject* o) {
    return _values->add(o);
}
