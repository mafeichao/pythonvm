#include "runtime/stringTable.hpp"
#include "object/hiString.hpp"

StringTable* StringTable::instance = NULL;

StringTable* StringTable::get_instance() {
    if (instance == NULL)
        instance = new StringTable();

    return instance;
}

StringTable::StringTable() {
    next_str = new HiString("__next__");
    init_str = new HiString("__init__");
    name_str = new HiString("__name__");
    _new_str = new HiString("__new__");
    call_str = new HiString("__call__");
    build_class_str = new HiString("__build_class__");
}

