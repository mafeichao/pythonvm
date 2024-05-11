#ifndef HI_LIST_HPP
#define HI_LIST_HPP

#include "object/klass.hpp"
#include "object/hiObject.hpp"
#include "object/arrayList.hpp"

class HiObject;
class OopClosure;

class ListKlass : public Klass {
private:
    ListKlass();
    static ListKlass* instance;

public:
    static ListKlass* get_instance();

    virtual HiObject* add(HiObject* x, HiObject* y);
    virtual HiObject* mul(HiObject* x, HiObject* y);

    virtual void print(HiObject* obj);
    virtual HiObject* subscr (HiObject* x, HiObject* y);
    virtual void store_subscr (HiObject* x, HiObject* y, HiObject* z);
    virtual void del_subscr (HiObject* x, HiObject* y);
    virtual HiObject* less     (HiObject* x, HiObject* y);
    virtual HiObject* iter(HiObject* x);
    virtual HiObject* contains (HiObject* x, HiObject* y);
};

class HiList : public HiObject {
friend class ListKlass;

private:
    ArrayList<HiObject*>* _inner_list;

public:
    HiList();
    HiList(ObjList ol);
    ArrayList<HiObject*>* inner_list()  { return _inner_list; }

    int length()                        { return _inner_list->length(); }
    void insert(int i, HiObject* obj)   { _inner_list->insert(i, obj); }
    void append(HiObject* obj)          { _inner_list->add(obj); }
    HiObject* pop()                     { return _inner_list->pop(); }
    HiObject* get(int index)            { return _inner_list->get(index); }
    void      set(int i, HiObject* o)   { _inner_list->set(i, o); }
    HiObject* top()                     { return get(length() - 1); }
    int       index(HiObject* obj)      { return _inner_list->index(obj); }
    void      clear()                   { _inner_list->clear(); }
};

HiObject* list_append(HiList* args);
HiObject* list_pop(HiList* args);
HiObject* list_remove(HiList* args);
HiObject* list_reverse(HiList* args);
HiObject* list_sort(HiList* args);
HiObject* list_extend(HiList* args);

class ListIteratorKlass : public Klass {
private:
    static ListIteratorKlass* instance;
    ListIteratorKlass();

public:
    static ListIteratorKlass* get_instance();
};

class ListIterator : public HiObject {
private:
    HiList*   _owner;
    int       _iter_cnt;
public:
    ListIterator(HiList* owner);

    HiList* owner()        { return _owner; }
    int iter_cnt()         { return _iter_cnt; }
    void inc_cnt()         { _iter_cnt++; }
};

HiObject* listiterator_next(HiList* args);

#endif

