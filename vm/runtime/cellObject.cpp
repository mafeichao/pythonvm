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

CellObject::CellObject(HiObject* o) : _value(o) {
    set_klass(CellKlass::get_instance());
}

size_t CellKlass::size() {
    return sizeof(CellObject);
}

void CellKlass::oops_do(OopClosure* f, HiObject* obj) {
    f->do_oop((HiObject**)&(obj->as<CellObject>()->_value));
}
