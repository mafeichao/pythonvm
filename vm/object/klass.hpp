#ifndef _KLASS_HPP
#define _KLASS_HPP

#include <stdio.h>

class HiObject;
class HiString;
class HiDict;
class HiList;
class HiTypeObject;
class OopClosure;
template<typename T> class Handle;

class Klass {
private:
    HiList*       _super;
    HiList*       _mro;
    HiTypeObject* _type_object;
    HiString*     _name;
    HiDict*       _klass_dict;

    HiList* linear(HiTypeObject* obj);
    HiList* merge(Handle<HiList*> supers);
    HiObject* find_in_mro(HiObject* obj, HiString* name);

public:
    Klass();

    static HiObject* create_klass(HiDict* x, HiList* supers, HiString* name);

    void order_supers();
    void add_super(Klass* x);
    HiList* super()                       { return _super; }
    void set_super_list(HiList* x)        { _super = x; }
    HiList* mro()                         { return _mro; }

    void set_type_object(HiTypeObject* x) { _type_object = x; }
    HiTypeObject* type_object()           { return _type_object; }

    void set_name(HiString* x)            { _name = x; }
    HiString* name()                      { return _name; }

    void set_klass_dict(HiDict* dict)     { _klass_dict = dict; }
    HiDict* klass_dict()                  { return _klass_dict; }

    virtual void print(HiObject* obj);
    virtual HiObject* len(HiObject* obj); 
    virtual HiObject* repr(HiObject* obj); 

    virtual HiObject* greater  (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* less     (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* equal    (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* not_equal(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* ge       (HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* le       (HiObject* x, HiObject* y) { return nullptr; }

    virtual HiObject* add(HiObject* x, HiObject* y);
    virtual HiObject* sub(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* mul(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* div(HiObject* x, HiObject* y) { return nullptr; }
    virtual HiObject* mod(HiObject* x, HiObject* y) { return nullptr; }

    virtual HiObject* iter(HiObject* x)                { return nullptr; }
    virtual HiObject* call     (HiObject* x, HiList* args, HiDict* kwargs);

    virtual HiObject* getattr(HiObject* x, HiObject* y);
    virtual HiObject* setattr(HiObject* obj, HiObject* x, HiObject* y);
    virtual HiObject* subscr (HiObject* x, HiObject* y);
    virtual void store_subscr  (HiObject* x, HiObject* y, HiObject* z);
    virtual void del_subscr    (HiObject* x, HiObject* y);
    virtual HiObject* contains (HiObject* x, HiObject* y)               { return nullptr; }

    virtual HiObject* allocate_instance(HiList* args);
    HiObject* find_and_call(HiObject* lhs, HiList* args, HiObject* func_name);

    // gc interfaces
    // this is for objects of this type.
    virtual void oops_do(OopClosure* closure, HiObject* obj);
    // for klass itself only.
    virtual void oops_do(OopClosure* closure);
    virtual size_t size();
    void* operator new(size_t size);
};

#endif

