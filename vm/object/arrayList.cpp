#include "arrayList.hpp"
#include <stdio.h>

template <typename T>
ArrayList<T>::ArrayList(int n) {
    _capacity = n;
    _length   = 0;
    _array  = new T[n];
}

template <typename T>
void ArrayList<T>::add(T t) {
    if (_length >= _capacity)
        expand();

    _array[_length++] = t;
}

template <typename T>
void ArrayList<T>::insert(int index, T t) {
    add(NULL);

    for (int i = _length; i > index; i--) {
        _array[i] = _array[i - 1];
    }

    _array[index] = t;
}

template <typename T>
void ArrayList<T>::expand() {
    T* new_array = new T[_capacity << 1];
    for (int i = 0; i < _capacity; i++) {
        new_array[i] = _array[i];
    }
    delete[] _array;
    _array = new_array;

    _capacity <<= 1;
    printf("expand an array to %d, length is %d\n", _capacity, _length);
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

class HiObject;
template class ArrayList<HiObject*>;

class HiString;
template class ArrayList<HiString*>;

