#include "arrayList.hpp"
#include "object/hiObject.hpp"
#include "object/hiString.hpp"
#include "object/klass.hpp"
#include "runtime/universe.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"
#include <new>
#include <cstdio>

using namespace std;

template <typename T>
ArrayList<T>::ArrayList(int n) {
    _capacity = n;
    _length   = 0;
    void* temp = Universe::heap->allocate(sizeof(T) * (n << 1));
    _array  = new (temp)T[n];
}

template <typename T>
void ArrayList<T>::add(T t) {
    if (_length >= _capacity)
        expand();

    _array[_length++] = t;
}

template <typename T>
void ArrayList<T>::insert(int index, T t) {
    add((T)0);

    for (int i = _length - 1; i > index; i--) {
        _array[i] = _array[i - 1];
    }

    _array[index] = t;
}

template <typename T>
void ArrayList<T>::expand() {
    void* temp = Universe::heap->allocate(sizeof(T) * (_length << 1), false);
    T* new_array = new (temp)T[_length << 1];
    for (int i = 0; i < _length; i++) {
        new_array[i] = _array[i];
    }
    _array = new_array;
    _capacity <<= 1;
}

template <typename T>
int ArrayList<T>::length() {
    return _length;
}

template <typename T>
int ArrayList<T>::capacity() {
    return _capacity;
}

template <typename T>
T ArrayList<T>::get(int index) {
    return _array[index];
}

template <typename T>
void ArrayList<T>::set(int index, T t) {
    if (_length <= index)
        _length = index + 1;

    while (_length > _capacity)
        expand();

    _array[index] = t;
}

template <typename T>
T ArrayList<T>::pop() {
    return _array[--_length];
}

template <typename T>
void ArrayList<T>::delete_index(int index) {
    for (int i = index; i + 1 < _length; i++) {
        _array[i] = _array[i+1];
    }
    _length--;
}

template <typename T>
void* ArrayList<T>::operator new(size_t size) {
    return Universe::heap->allocate(size);
}

template <typename T>
int ArrayList<T>::index(T t) {
    for (int i = 0; i < _length; i++) {
        if (_array[i]->equal(t) == Universe::HiTrue) {
            return i;
        }
    }

    return -1;
}

template <typename T>
void ArrayList<T>::clear() {
    _length = 0;
}

class HiObject;
template class ArrayList<HiObject*>;

class HiString;
template class ArrayList<HiString*>;

template<>
int ArrayList<Klass*>::index(Klass* t) {
    for (int i = 0; i < _length; i++) {
        if (_array[i] == t) {
            return i;
        }
    }

    return -1;
}

template <>
int ArrayList<char>::index(char t) {
    for (int i = 0; i < _length; i++) {
        if (_array[i] == t) {
            return i;
        }
    }

    return -1;
}

template <typename T>
void ArrayList<T>::oops_do(OopClosure* closure) {
    closure->do_raw_mem((char**)(&_array),
            _capacity * sizeof(T));
}

template <>
void ArrayList<Klass*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem((char**)(&_array),
            _capacity * sizeof(Klass*));

    for (int i = 0; i < _length; i++) {
        closure->do_klass((Klass**)&_array[i]);
    }
    return;
}

template <>
void ArrayList<HiObject*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem((char**)(&_array),
            _capacity * sizeof(HiObject*));

    for (int i = 0; i < _length; i++) {
        closure->do_oop((HiObject**)&_array[i]);
    }
}

class Klass;
template class ArrayList<Klass*>;

template class ArrayList<char>;
