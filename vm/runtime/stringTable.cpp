#include "runtime/stringTable.hpp"
#include "object/hiString.hpp"
#include "memory/oopClosure.hpp"

StringTable* StringTable::instance = NULL;

StringTable* StringTable::get_instance() {
    if (instance == NULL)
        instance = new StringTable();

    return instance;
}

void StringTable::destroy() {
    delete instance;
    instance = nullptr;
}

StringTable::StringTable() {
    add_str  = HiString::new_instance("__add__");
    build_class_str = HiString::new_instance("__build_class__");
    call_str = HiString::new_instance("__call__");
    delitem_str = HiString::new_instance("__delitem__");
    getitem_str = HiString::new_instance("__getitem__");
    getattr_str = HiString::new_instance("__getattr__");
    init_str = HiString::new_instance("__init__");
    len_str  = HiString::new_instance("__len__");
    mro_str  = HiString::new_instance("__mro__");
    name_str = HiString::new_instance("__name__");
    _new_str = HiString::new_instance("__new__");
    classcell_str = HiString::new_instance("__classcell__");
    class_str = HiString::new_instance("__class__");
    next_str = HiString::new_instance("__next__");
    repr_str = HiString::new_instance("__repr__");
    setattr_str = HiString::new_instance("__setattr__");
    setitem_str = HiString::new_instance("__setitem__");
    str_str  = HiString::new_instance("__str__");
    tb_str  = HiString::new_instance("__traceback__");
    lib_str  = HiString::new_instance("lib/");
    pyc_str  = HiString::new_instance(".pyc");

#ifdef HIVM_APPLE
    so_str  = HiString::new_instance(".dylib");
#elif defined(HIVM_WINDOWS) || defined(HIVM_MINGW)
    so_str  = HiString::new_instance(".dll");
#else
    so_str  = HiString::new_instance(".so");
#endif

    div_zero_str = HiString::new_instance("ZeroDivisionError");
    stop_iter_str = HiString::new_instance("StopIteration");
    index_err_str = HiString::new_instance("IndexError");;
    exc_str = HiString::new_instance("Exception");
    context_str = HiString::new_instance("__context__");
}

void StringTable::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&add_str);
    f->do_oop((HiObject**)&build_class_str);
    f->do_oop((HiObject**)&call_str);
    f->do_oop((HiObject**)&delitem_str);
    f->do_oop((HiObject**)&getitem_str);
    f->do_oop((HiObject**)&getattr_str);
    f->do_oop((HiObject**)&init_str);
    f->do_oop((HiObject**)&len_str);
    f->do_oop((HiObject**)&mro_str);
    f->do_oop((HiObject**)&_new_str);
    f->do_oop((HiObject**)&classcell_str);
    f->do_oop((HiObject**)&class_str);
    f->do_oop((HiObject**)&name_str);
    f->do_oop((HiObject**)&next_str);
    f->do_oop((HiObject**)&repr_str);
    f->do_oop((HiObject**)&setitem_str);
    f->do_oop((HiObject**)&setattr_str);
    f->do_oop((HiObject**)&str_str);
    f->do_oop((HiObject**)&lib_str);
    f->do_oop((HiObject**)&pyc_str);
    f->do_oop((HiObject**)&so_str);
    f->do_oop((HiObject**)&tb_str);

    //Exceptions;
    f->do_oop((HiObject**)&div_zero_str);
    f->do_oop((HiObject**)&stop_iter_str);
    f->do_oop((HiObject**)&index_err_str);
    f->do_oop((HiObject**)&exc_str);
    f->do_oop((HiObject**)&context_str);
}

