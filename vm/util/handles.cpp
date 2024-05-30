#include "util/handles.hpp"
#include "object/arrayList.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "runtime/frameObject.hpp"
#include "memory/oopClosure.hpp"

#include <stdio.h>

HandleMark* HandleMark::instance = NULL;

HandleMark::HandleMark() {
    _head = 0x0;
}

HandleMark* HandleMark::get_instance() {
    if (!instance)
        instance = new HandleMark();

    return instance;
}

void HandleMark::oops_do(OopClosure* f) {
    LinkedList* cur = _head;
    while (cur) {
        cur->oops_do(f);
        cur = cur->_next;
    }
}

void HandleMark::set_head(LinkedList* node) {
    if (_head) {
        _head->_prev = node;
    }
    node->_next = _head;
    _head = node;
}

void HandleMark::del_handle(LinkedList* node) {
    LinkedList* prev = node->_prev;
    LinkedList* next = node->_next;

    if (prev) {
        prev->_next = next;
    }

    if (next) {
        next->_prev = prev;
    }

    if (_head == node) {
        _head = next;
    }
}

LinkedList::LinkedList() {
    _next = NULL;
    _prev = NULL;
}

template<typename T>
Handle<T>::Handle(T t) : LinkedList() {
    _value = t;
    HandleMark::get_instance()->set_head(this);
}

template<typename T>
Handle<T>::Handle(const Handle<T>& t) : LinkedList() {
    _value = t._value;
    HandleMark::get_instance()->set_head(this);
}

template<typename T>
Handle<T>::~Handle() {
    _value = 0x0;
    HandleMark::get_instance()->del_handle(this);
}

template<typename T>
T Handle<T>::operator ->() {
    return _value;
}

template<typename T>
void Handle<T>::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&_value);
}

template<>
void Handle<ArrayList<HiObject*>*>::oops_do(OopClosure* f) {
    f->do_array_list(&_value);
}

template<>
void Handle<ArrayList<HiString*>*>::oops_do(OopClosure* f) {
    f->do_array_list(&_value);
}

template<>
void Handle<ArrayList<Klass*>*>::oops_do(OopClosure* f) {
    f->do_array_list(&_value);
}

template<>
void Handle<Map<HiObject*, HiObject*>*>::oops_do(OopClosure* f) {
    f->do_map(&_value);
}

class HiObject;
template class Handle<HiObject*>;

class HiInteger;
template class Handle<HiInteger*>;

class HiString;
template class Handle<HiString*>;

class HiList;
template class Handle<HiList*>;

class HiDict;
template class Handle<HiDict*>;

class CodeObject;
template class Handle<CodeObject*>;

class FunctionObject;
template class Handle<FunctionObject*>;

class MethodObject;
template class Handle<MethodObject*>;

class HiTypeObject;
template class Handle<HiTypeObject*>;

class ModuleObject;
template class Handle<ModuleObject*>;

class Traceback;
template class Handle<Traceback*>;

template class Handle<ArrayList<HiObject*>*>;
template class Handle<ArrayList<Klass*>*>;
template class Handle<ArrayList<HiString*>*>;

template class Handle<Map<HiObject*, HiObject*>*>;
