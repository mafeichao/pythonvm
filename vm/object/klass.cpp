#include "object/klass.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiString.hpp"
#include "object/typeObject.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"

Klass::Klass() {
    Universe::klasses->add(this);
    _super          = nullptr;
    _mro            = nullptr;
    _type_object    = nullptr;
    _name           = nullptr;
    _klass_dict     = nullptr;
}

void Klass::add_super(Klass* klass) {
    if (_super == nullptr)
        _super = HiList::new_instance();

    _super->append(klass->type_object());
}

HiList* Klass::linear(HiTypeObject* obj) {
    Handle<HiList*> mro = obj->mro();
    Handle<HiList*> result = HiList::new_instance();
    for (int i = 0; i < mro->length(); i++) {
        result->append(mro->get(i));
    }

    return result;
}

HiList* Klass::merge(Handle<HiList*> supers) {
    if (supers->empty()) {
        return HiList::new_instance();
    }

    for (int i = 0; i < supers->length(); i++) {
        bool valid = true;
        // head = supers[i][0]
        Handle<HiTypeObject*> head = supers->get(i)->as<HiList>()->get(0)->as<HiTypeObject>();
        for (int j = 0; j < supers->length(); j++) {
            if (j == i) continue;
            // if head in supers[j][1:]
            if (supers->get(j)->as<HiList>()->index(head) > 0) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            continue;
        }

        Handle<HiList*> next = HiList::new_instance();
        for (int j = 0; j < supers->length(); j++) {
            // 在 itme 的生命周期中，不会发生GC
            HiList* item = supers->get(j)->as<HiList>();
            item->remove(head);
            if (!item->empty()) {
                next->append(item);
            }
        }
        Handle<HiList*> result = merge(next);
        result->insert(0, head);

        return result;
    }

    printf("Should not reach here\n");
    assert(false);
    return nullptr;
}

void Klass::order_supers() {
    if (_super == nullptr) {
        _mro = HiList::new_instance();
        _mro->append(_type_object);
        return;
    }

    Handle<HiList*> all = HiList::new_instance();
    for (int i = 0; i < _super->length(); i++) {
        all->append(linear(_super->get(i)->as<HiTypeObject>()));
    }

    _mro = merge(all);
    _mro->insert(0, _type_object);
    _klass_dict->put(ST(mro), _mro);
}

void Klass::print(HiObject* x) {
    HiObject* func = x->getattr(ST(str));
    if (func != Universe::HiNone) {
        HiObject* s = Interpreter::get_instance()->call_virtual(func, nullptr);
        s->as<HiString>()->print();
        return;
    }

    func = x->getattr(ST(repr));
    if (func != Universe::HiNone) {
        HiObject* r = Interpreter::get_instance()->call_virtual(func, nullptr);
        r->as<HiString>()->print();
        return;
    }

    printf("<object at %p>", x);
}

HiObject* Klass::call(HiObject* x, HiList* args, HiDict* kwargs) {
    HiObject* callable = x->getattr(ST(call));

    if (callable == Universe::HiNone) {
        x->print();
        printf(" is non-callable\n");
        assert(false);
    }

    return Interpreter::get_instance()->call_virtual(callable, args);
}

HiObject* Klass::find_in_mro(HiObject* obj, HiString* name) {
    HiObject* x = obj->klass()->klass_dict()->get(name);

    if (x != Universe::HiNone) {
        return x;
    }

    for (int i = 0; i < obj->klass()->mro()->length(); i++) {
        x = obj->klass()->mro()->get(i)->getattr(name);
        if (x != Universe::HiNone) {
            return x;
        }
    }

    return x;
}

// getattr for normal object.
// a = A()
// a.b = 1
HiObject* Klass::getattr(HiObject* rawx, HiObject* rawy) {
    Handle<HiObject*> x(rawx);
    Handle<HiObject*> y(rawy);

    Handle<HiObject*> func = find_in_mro(x, ST(getattr));

    // 如果类里定义了__getattr__方法，就先调用这个方法
    if (func != Universe::HiNone) {
        func = new MethodObject(func->as<FunctionObject>(), x);
        Handle<HiList*> args = HiList::new_instance();
        args->append(y);
        return Interpreter::get_instance()->call_virtual(func, args);
    }

    // 如果没有定义 __getattr__方法，就去对象字典里找
    if (x->obj_dict()->has_key(y)) {
        return x->obj_dict()->get(y);
    }

    // 如果对象字典里也没有，就去类里找
    Handle<HiObject*> result = find_in_mro(x, y->as<HiString>());
    // Only klass attribute needs bind.
    if (MethodObject::is_function(result) ||
        MethodObject::is_native(result)) {
        result = new MethodObject(result->as<FunctionObject>(), x);
    }

    return result;
}

// setattr for normal object.
HiObject* Klass::setattr(HiObject* obj, HiObject* x, HiObject* y) {
    HiObject* func = obj->klass()->klass_dict()->get(ST(setattr));

    // 如果未定义__setattr__方法，就直接放到对象的obj_dict中
    if (func == Universe::HiNone) {
        obj->obj_dict()->put(x, y);
        return Universe::HiNone;
    }

    func = new MethodObject(func->as<FunctionObject>(), obj);
    HiList* args = HiList::new_instance();
    args->append(x);
    args->append(y);
    return Interpreter::get_instance()->call_virtual(func, args);
}

HiObject* Klass::subscr(HiObject* x, HiObject* y) {
    HiList* args = HiList::new_instance();
    args->append(y);
    return find_and_call(x, args, ST(getitem));
}

void Klass::store_subscr(HiObject* x, HiObject* y, HiObject* z) {
    HiList* args = HiList::new_instance();
    args->append(y);
    args->append(z);
    find_and_call(x, args, ST(setitem));
}

void Klass::del_subscr(HiObject* x, HiObject* y) {
    HiList* args = HiList::new_instance();
    args->append(y);
    find_and_call(x, args, ST(delitem));
}

HiObject* Klass::create_klass(HiDict* klass_dict, HiList* supers_list, HiString* name) {
    Klass* new_klass   = new Klass();

    new_klass->set_klass_dict(klass_dict);
    new_klass->set_name(name);

    if (supers_list->length() == 0) {
        supers_list->append(TypeKlass::get_instance()->type_object());
    }
    new_klass->set_super_list(supers_list);

    HiTypeObject* type_obj = new HiTypeObject();
    type_obj->set_own_klass(new_klass);

    new_klass->order_supers();
    
    return type_obj;
}

HiObject* Klass::allocate_instance(HiList* raw_args) {
    Handle<HiList*> args(raw_args);
    Handle<HiObject*> inst = new HiObject();
    inst->set_klass(this);
    Handle<HiObject*> init_func = inst->getattr(ST(init));

    if (init_func != Universe::HiNone) {
        Interpreter::get_instance()->call_virtual(init_func, args);
    }

    return inst;
}

HiObject* Klass::add(HiObject* raw_lhs, HiObject* raw_rhs) {
    Handle<HiObject*> lhs(raw_lhs);
    Handle<HiObject*> rhs(raw_rhs);
    Handle<HiList*> args = HiList::new_instance();
    args->append(rhs);

    return find_and_call(lhs, args, ST(add));
}

HiObject* Klass::len(HiObject* obj) {
    return find_and_call(obj, nullptr, ST(len));
}

HiObject* Klass::repr(HiObject* obj) {
    return find_and_call(obj, nullptr, ST(repr));
}

HiObject* Klass::find_and_call(HiObject* lhs, HiList* args, HiObject* func_name) {
    HiObject* func = lhs->getattr(func_name);
    if (func != Universe::HiNone) {
        return Interpreter::get_instance()->call_virtual(func, args);
    }

    printf("class ");
    lhs->klass()->name()->print();
    printf(" Error : unsupport operation for class ");
    assert(false);
    return Universe::HiNone;
}

// this function will visit all children
void Klass::oops_do(OopClosure* closure, HiObject* obj) {
    printf("warning: klass oops_do for ");
    _name->print();
    printf("\n");
}

void Klass::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&_super);
    f->do_oop((HiObject**)&_mro);
    f->do_oop((HiObject**)&_name);
    f->do_oop((HiObject**)&_klass_dict);
    f->do_oop((HiObject**)&_type_object);
}

size_t Klass::size() {
    return sizeof(HiObject);
}

void* Klass::operator new(size_t size) {
    return Universe::heap->allocate_meta(size);
}

