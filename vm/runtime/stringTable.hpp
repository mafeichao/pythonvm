#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

#define ST(x) (StringTable::get_instance()->x##_str)

class HiString;
class OopClosure;

class StringTable {
private:
    static StringTable* instance;
    StringTable();

public:
    static StringTable* get_instance();
    static void destroy();
    
    HiString* add_str;
    HiString* build_class_str;
    HiString* call_str;
    HiString* delitem_str;
    HiString* getitem_str;
    HiString* getattr_str;
    HiString* init_str;
    HiString* len_str;
    HiString* mro_str;
    HiString* name_str;
    HiString* _new_str;
    HiString* next_str;
    HiString* repr_str;
    HiString* setitem_str;
    HiString* setattr_str;
    HiString* str_str;
    HiString* lib_str;
    HiString* pyc_str;
    HiString* so_str;

    void oops_do(OopClosure* f);
};

#endif

