#include "runtime/stringTable.hpp"
#include "object/hiString.hpp"

StringTable* StringTable::instance = NULL;

StringTable* StringTable::get_instance() {
    if (instance == NULL)
        instance = new StringTable();

    return instance;
}

StringTable::StringTable() {
    add_str  = new HiString("__add__");
    build_class_str = new HiString("__build_class__");
    call_str = new HiString("__call__");
    init_str = new HiString("__init__");
    len_str  = new HiString("__len__");
    name_str = new HiString("__name__");
    _new_str = new HiString("__new__");
    next_str = new HiString("__next__");
    repr_str = new HiString("__repr__");
    str_str  = new HiString("__str__");
}

