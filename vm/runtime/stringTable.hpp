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
    
    HiString* add_str;
    HiString* build_class_str;
    HiString* call_str;
    HiString* delitem_str;
    HiString* getitem_str;
    HiString* getattr_str;
    HiString* init_str;
    HiString* len_str;
    HiString* name_str;
    HiString* _new_str;
    HiString* next_str;
    HiString* repr_str;
    HiString* setitem_str;
    HiString* setattr_str;
    HiString* str_str;
};

#endif

