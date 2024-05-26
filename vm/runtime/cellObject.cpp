#include "runtime/cellObject.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "object/hiString.hpp"

CellKlass* CellKlass::_instance = NULL;

CellKlass* CellKlass::get_instance() {
    if (_instance == NULL) {
        _instance = new CellKlass();
    }

    return _instance;
}

CellKlass::CellKlass() {
    set_klass_dict(HiDict::new_instance());
    set_name(HiString::new_instance("cell"));
}

CellObject::CellObject(HiList* t, int i) : 
        _table(t), _index(i) {
    set_klass(CellKlass::get_instance());
}

HiObject* CellObject::value() {
    return _table->get(_index);
}

void CellObject::set_value(HiObject* o) {
    return _table->set(_index, o);
}

size_t CellKlass::size() {
    return sizeof(CellKlass);
}

