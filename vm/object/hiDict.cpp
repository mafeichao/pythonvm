#include "object/hiDict.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/typeObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/stringTable.hpp"
#include "memory/oopClosure.hpp"
#include "util/handles.hpp"
#include <assert.h>

DictKlass* DictKlass::instance = NULL;

DictKlass* DictKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictKlass();
    }

    return instance;
}

DictKlass::DictKlass() {
}

void DictKlass::initialize() {
    HiDict* klass_dict = HiDict::new_instance();

    HiString* name = HiString::new_instance("setdefault");
    klass_dict->put(name, new FunctionObject(dict_set_default, name));
    name = HiString::new_instance("pop");
    klass_dict->put(name, new FunctionObject(dict_pop, name));
    name = HiString::new_instance("get");
    klass_dict->put(name, new FunctionObject(dict_get, name));
    name = HiString::new_instance("keys");
    klass_dict->put(name, new FunctionObject(dict_keys, name));
    name = HiString::new_instance("values");
    klass_dict->put(name, new FunctionObject(dict_values, name));
    name = HiString::new_instance("items");
    klass_dict->put(name, new FunctionObject(dict_items, name));

    set_klass_dict(klass_dict);
    (new HiTypeObject())->set_own_klass(this);
    set_name(HiString::new_instance("dict"));

    add_super(ObjectKlass::get_instance());
    order_supers();
}

void DictKlass::print(HiObject* obj) {
    HiDict* dict_obj = (HiDict*) obj;
    assert(dict_obj && dict_obj->klass() == (Klass*) this);

    printf("{");   
    int length = dict_obj->length();
    if (length >= 1) {
        dict_obj->_map->entries()[0]._k->print();
        printf(":");   
        dict_obj->_map->entries()[0]._v->print();
    }

    for (int i = 1; i < length; i++) {
        printf(", ");   
        dict_obj->_map->entries()[i]._k->print();
        printf(":");   
        dict_obj->_map->entries()[i]._v->print();
    }

    printf("}");
}

size_t DictKlass::size() {
    return sizeof(HiDict);
}

HiObject* DictKlass::getattr(HiObject* obj, HiString* name) {
    assert(obj->klass() == (Klass*) this);
    return klass_dict()->get((HiObject*)name);
}

HiObject* DictKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == (Klass*) this);
    return ((HiDict*)x)->map()->get(y);
}

void DictKlass::store_subscr(HiObject* x, HiObject*y, HiObject* z) {
    assert(x && x->klass() == (Klass*) this);
    ((HiDict*)x)->put(y, z);
}

void DictKlass::del_subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == (Klass*) this);
    ((HiDict*)x)->remove(y);
}

HiObject* DictKlass::contains(HiObject* x, HiObject* y) {
    if (x->as<HiDict>()->has_key(y)) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* DictKlass::allocate_instance(HiList* args) {
    if (!args || args->length() == 0)
        return HiDict::new_instance();
    else
        return nullptr;
}

HiObject* DictKlass::iter(HiObject* x) {
    return new DictIterator((HiDict*)x);
}

HiDict::HiDict() {
    _map = nullptr;
    set_klass(DictKlass::get_instance());
}

HiDict* HiDict::new_instance() {
    Handle<HiDict*> result = new HiDict();
    Map<HiObject*, HiObject*>* map = Map<HiObject*, HiObject*>::new_instance(DEFAULT_SIZE);
    result->_map = map;
    return result;
}

HiDict::HiDict(Map<HiObject*, HiObject*>* x) {
    _map = x;
    set_klass(DictKlass::get_instance());
}

void HiDict::put(HiObject* k, HiObject* v) {
    _map->put(k, v);
}

/*
 * Iterations for dict object
 */
DictIterator::DictIterator(HiDict* dict) {
    _owner = dict;
    _iter_cnt = 0;
    set_klass(DictIteratorKlass::get_instance());
}

DictIteratorKlass* DictIteratorKlass::instance = NULL;

DictIteratorKlass* DictIteratorKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictIteratorKlass();
    }

    return instance;
}

DictIteratorKlass::DictIteratorKlass() {
    HiDict* klass_dict = HiDict::new_instance();
    klass_dict->put(ST(next),
            new FunctionObject(dictiterator_next, ST(next)));
    set_klass_dict(klass_dict);
    (new HiTypeObject())->set_own_klass(this);
    set_name(HiString::new_instance("dictiterator"));
    add_super(ObjectKlass::get_instance());
    order_supers();
}

void DictIteratorKlass::oops_do(OopClosure* f, HiObject* obj) {
    f->do_oop((HiObject**)((DictIterator*)obj)->owner_address());
}

size_t DictIteratorKlass::size() {
    return sizeof(DictIterator);
}

HiObject* dict_set_default(HiList* args, HiDict* kwargs) {
    HiDict* dict = (HiDict*)(args->get(0));
    HiObject* key = args->get(1);
    HiObject* value = args->get(2);

    if (!dict->has_key(key))
        dict->put(key, value);

    return Universe::HiNone;
}

HiObject* dict_pop(HiList* args, HiDict* kwargs) {
    HiDict* x = (HiDict*)args->get(0);
    HiObject* y = args->get(1);
    HiObject* z = Universe::HiNone;

    if (x->has_key(y)) {
        z = x->get(y);
        x->remove(y);
    }
    else {
        if (args->length() == 3) {
            z = args->get(2);
        }
    }

    return z;
}

HiObject* dict_get(HiList* args, HiDict* kwargs) {
    return args->get(0)->as<HiDict>()->get(args->get(1));
}

HiObject* dict_keys(HiList* args, HiDict* kwargs) {
    HiDict* x = (HiDict*)(args->get(0));
    HiObject* it = new DictView(x);
    it->set_klass(DictViewKlass<ITER_KEY>::get_instance());
    return it;
}

HiObject* dict_values(HiList* args, HiDict* kwargs) {
    HiDict* x = (HiDict*)(args->get(0));
    HiObject* it = new DictView(x);
    it->set_klass(DictViewKlass<ITER_VALUE>::get_instance());
    return it;
}

HiObject* dict_items(HiList* args, HiDict* kwargs) {
    HiDict* x = (HiDict*)(args->get(0));
    HiObject* it = new DictView(x);
    it->set_klass(DictViewKlass<ITER_ITEM>::get_instance());
    return it;
}

HiObject* dictiterator_next(HiList* args, HiDict* kwargs) {
    DictIterator* iter = (DictIterator*)(args->get(0));

    HiDict* adict = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if (iter_cnt < adict->map()->length()) {
        HiObject* obj = adict->map()->get_key(iter_cnt);
        iter->inc_cnt();
        return obj;
    }
    else // TODO : we need Traceback here to mark iteration end
        return nullptr;
}

template<ITER_TYPE n>
DictViewKlass<n>* DictViewKlass<n>::instance = NULL;

template<ITER_TYPE n>
DictViewKlass<n>* DictViewKlass<n>::get_instance() {
    if (instance == NULL) {
        instance = new DictViewKlass<n>();
    }

    return instance;
}

template<ITER_TYPE iter_type>
DictViewKlass<iter_type>::DictViewKlass() {
    const char* klass_names[] = {
        "dict_keys",
        "dict_values",
        "dict_items",
    };
    HiDict* klass_dict = HiDict::new_instance();
    klass_dict->put(ST(next),
            new FunctionObject(dict_view_next<iter_type>, ST(next)));
    set_klass_dict(klass_dict);
    (new HiTypeObject())->set_own_klass(this);
    set_name(HiString::new_instance(klass_names[iter_type]));
    add_super(ObjectKlass::get_instance());
    order_supers();
}

template<ITER_TYPE iter_type>
void DictViewKlass<iter_type>::oops_do(OopClosure* f, HiObject* obj) {
    f->do_oop(((DictView*)obj)->owner_address());
}

template<ITER_TYPE iter_type>
size_t DictViewKlass<iter_type>::size() {
    return sizeof(DictView);
}

DictView::DictView(HiDict* dict) {
    _owner = dict;
    _iter_cnt = 0;
}

template<ITER_TYPE iter_type>
HiObject* dict_view_next(HiList* args, HiDict* kwargs) {
    DictIterator* iter = (DictIterator*)(args->get(0));

    HiDict* adict = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if (iter_cnt < adict->map()->length()) {
        HiObject* obj;
        if (iter_type == ITER_KEY)
            obj = adict->map()->get_key(iter_cnt);
        else if (iter_type == ITER_VALUE) {
            obj = adict->map()->get_value(iter_cnt);
        }
        else if (iter_type == ITER_ITEM) {
            HiList* lobj = HiList::new_instance();
            lobj->append(adict->map()->get_key(iter_cnt));
            lobj->append(adict->map()->get_value(iter_cnt));
            obj = lobj;
        }
        iter->inc_cnt();
        return obj;
    }
    else // TODO : we need Traceback here to mark iteration end
        return NULL;
}

template<ITER_TYPE iter_type>
HiObject* DictViewKlass<iter_type>::contains(HiObject* x, HiObject* y) {
    assert(x->klass() == DictViewKlass<iter_type>::get_instance());
    HiDict* adict = ((DictView*)x)->owner()->as<HiDict>();

    bool flag = false;
    if (iter_type == ITER_KEY) {
        flag = adict->map()->has_key(y);
    }
    else if (iter_type == ITER_VALUE) {
    }
    else if (iter_type == ITER_ITEM) {
    }

    if (flag) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

void DictKlass::oops_do(OopClosure* f, HiObject* obj) {
    f->do_map(&obj->as<HiDict>()->_map);
}
