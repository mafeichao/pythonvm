#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/interpreter.hpp"
#include <assert.h>

ListKlass* ListKlass::instance = nullptr;

ListKlass* ListKlass::get_instance() {
    if (instance == nullptr)
        instance = new ListKlass();

    return instance;
}

void ListKlass::initialize() {
    HiDict * klass_dict = new HiDict();
    klass_dict->put(new HiString("append"), 
        new FunctionObject(list_append));
    klass_dict->put(new HiString("index"), 
        new FunctionObject(list_index));
    klass_dict->put(new HiString("pop"),
        new FunctionObject(list_pop));
    klass_dict->put(new HiString("remove"),
        new FunctionObject(list_remove));
    klass_dict->put(new HiString("reverse"),
        new FunctionObject(list_reverse));
    klass_dict->put(new HiString("sort"),
        new FunctionObject(list_sort));
    klass_dict->put(new HiString("extend"),
        new FunctionObject(list_extend));

    set_klass_dict(klass_dict);
    (new HiTypeObject())->set_own_klass(this);
    set_name(new HiString("list"));

    add_super(ObjectKlass::get_instance());
    order_supers();
}

ListKlass::ListKlass() {
}

HiObject* ListKlass::add(HiObject* x, HiObject* y) {
    HiList* lx = x->as<HiList>();
    HiList* ly = y->as<HiList>();

    HiList* z = new HiList();
    for (int i = 0; i < lx->length(); i++) {
        z->inner_list()->set(i, lx->inner_list()->get(i));
    }

    for (int i = 0; i < ly->length(); i++) {
        z->inner_list()->set(i + lx->length(),
                ly->inner_list()->get(i));
    }

    return z;
}

HiObject* ListKlass::mul(HiObject* x, HiObject* y) {
    HiList* lx = x->as<HiList>();
    HiInteger* iy = y->as<HiInteger>();

    HiList* z = new HiList();
    for (int i = 0; i < iy->value(); i++) {
        for (int j = 0; j < lx->length(); j++) {
            z->inner_list()->set(i * lx->length() + j,
                    lx->inner_list()->get(j));
        }
    }

    return z;
}

HiObject* ListKlass::len(HiObject* x) {
    return new HiInteger(x->as<HiList>()->length());
}

void ListKlass::print(HiObject* x) {
    HiList * lx = x->as<HiList>();

    printf("[");

    int size = lx->_inner_list->length();
    if (size >= 1)
        lx->_inner_list->get(0)->print();

    for (int i = 1; i < size; i++) {
        printf(", ");
        lx->_inner_list->get(i)->print();
    }
    printf("]");
}

HiObject* ListKlass::subscr(HiObject* x, HiObject* y) {
    HiList * lx = x->as<HiList>();
    HiInteger* iy = y->as<HiInteger>();
    
    return lx->inner_list()->get(iy->value());
}

void ListKlass::store_subscr(HiObject* x, HiObject* y, HiObject* z) {
    HiList * lx = x->as<HiList>();
    HiInteger* iy = y->as<HiInteger>();

    lx->inner_list()->set(iy->value(), z);
}

void ListKlass::del_subscr(HiObject* x, HiObject* y) {
    HiList * lx = x->as<HiList>();
    HiInteger* iy = y->as<HiInteger>();

    lx->inner_list()->delete_index(iy->value());
}

HiObject* ListKlass::less(HiObject* x, HiObject* y) {
    HiList* lx = x->as<HiList>();
    HiList* ly = y->as<HiList>();

    int len = lx->length() < ly->length() ?
        lx->length() : ly->length();

    for (int i = 0; i < len; i++) {
        if (lx->get(i)->less(ly->get(i)) == Universe::HiTrue) {
            return Universe::HiTrue;
        }
        else if (lx->get(i)->equal(ly->get(i)) != Universe::HiTrue) {
            return Universe::HiFalse;
        }
    }

    if (lx->length() < ly->length())
        return Universe::HiTrue;

    return Universe::HiFalse;
}

HiObject* ListKlass::contains(HiObject* x, HiObject* y) {
    HiList * lx = x->as<HiList>();

    int size = lx->_inner_list->length();
    for (int i = 1; i < size; i++) {
        if (lx->_inner_list->get(i)->equal(y))
            return Universe::HiTrue;
    }

    return Universe::HiFalse;
}

HiObject* ListKlass::allocate_instance(HiList* args) {
    if (!args || args->length() == 0)
        return new HiList();
    else
        return nullptr;
}

HiObject* ListKlass::iter(HiObject* x) {
    return new ListIterator(x->as<HiList>());
}

HiList::HiList() {
    set_klass(ListKlass::get_instance());
    _inner_list = new ArrayList<HiObject*>();
}

HiList::HiList(ObjList ol) {
    set_klass(ListKlass::get_instance());
    _inner_list = ol;
}

void HiList::remove(HiObject* o) {
    int index = _inner_list->index(o);
    if (index >= 0) {
        _inner_list->delete_index(index);
    }
}

HiObject* list_append(HiList* args, HiDict* kwargs) {
    ((HiList*)(args->get(0)))->append(args->get(1));
    return Universe::HiNone;
}

HiObject* list_index(HiList* args, HiDict* kwargs) {
    return new HiInteger(args->get(0)->as<HiList>()->index(args->get(1)));
}

HiObject* list_pop(HiList* args, HiDict* kwargs) {
    return args->get(0)->as<HiList>()->pop();
}

HiObject* list_remove(HiList* args, HiDict* kwargs) {
    HiList* list = args->get(0)->as<HiList>();
    HiObject* target = args->get(1);
    list->remove(target);

    return Universe::HiNone;
}

HiObject* list_reverse(HiList* args, HiDict* kwargs) {
    HiList* list = args->get(0)->as<HiList>();

    int i = 0;
    int j = list->length() - 1;
    while (i < j) {
        HiObject* t = list->get(i);
        list->set(i, list->get(j));
        list->set(j, t);

        i++;
        j--;
    }

    return Universe::HiNone;
}

HiObject* list_sort(HiList* args, HiDict* kwargs) {
    HiList* list = args->get(0)->as<HiList>();

    // bubble sort
    for (int i = 0; i < list->length(); i++) {
        for (int j = list->length() - 1; j > i; j--) {
            if (list->get(j)->less(list->get(j-1)) == Universe::HiTrue) {
                HiObject* t = list->get(j);
                list->set(j, list->get(j-1));
                list->set(j-1, t);
            }
        }
    }

    return Universe::HiNone;
}

HiObject* list_extend(HiList* args, HiDict* kwargs) {
    HiList* lx = (HiList*)(args->get(0));
    HiObject* obj = args->get(1);

    HiObject* next_func = obj->iter()->getattr(StringTable::get_instance()->next_str);
    assert(next_func != Universe::HiNone);

    HiObject* to;
    while ((to = Interpreter::get_instance()->call_virtual(next_func, nullptr)) != nullptr) {
        lx->append(to);
    }

    return Universe::HiNone;
}

/*
 * List Iterators
 */
ListIteratorKlass* ListIteratorKlass::instance = nullptr;

ListIteratorKlass* ListIteratorKlass::get_instance() {
    if (instance == nullptr)
        instance = new ListIteratorKlass();

    return instance;
}

ListIteratorKlass::ListIteratorKlass() {
    HiDict* klass_dict = new HiDict();
    klass_dict->put(StringTable::get_instance()->next_str,
            new FunctionObject(listiterator_next));
    set_klass_dict(klass_dict);
}

ListIterator::ListIterator(HiList* list) {
    _owner = list;
    _iter_cnt = 0;
    set_klass(ListIteratorKlass::get_instance());
}

HiObject* listiterator_next(HiList* args, HiDict* kwargs) {
    ListIterator* iter = (ListIterator*)(args->get(0));

    HiList* alist = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if (iter_cnt < alist->inner_list()->length()) {
        HiObject* obj = alist->get(iter_cnt);
        iter->inc_cnt();
        return obj;
    }
    else // TODO : we need Traceback here to mark iteration end
        return NULL;
}

