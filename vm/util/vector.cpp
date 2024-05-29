#include "util/vector.hpp"
#include "runtime/frameObject.hpp"
#include <cstdio>
#include <cassert>

using namespace std;

template <typename T>
Vector<T>::Vector(int n) {
    assert(n > 0);
    _capacity = n;
    _length   = 0;
    _array  = new T[n];
}

template <typename T>
Vector<T>::~Vector() {
    _capacity = 0;
    _length   = 0;
    if (_array) {
        delete[] _array;
        _array = nullptr;
    }
}

template <typename T>
void Vector<T>::add(T t) {
    if (_length >= _capacity)
        expand();

    _array[_length++] = t;
}

template <typename T>
void Vector<T>::insert(int index, T t) {
    add(t);

    for (int i = _length - 1; i > index; i--) {
        _array[i] = _array[i - 1];
    }

    _array[index] = _array[_length - 1];
}

template <typename T>
void Vector<T>::expand() {
    assert(_capacity > 0 && _length >= _capacity);
    T* new_array = new T[_length << 1];
    for (int i = 0; i < _length; i++) {
        new_array[i] = _array[i];
    }
    delete[] _array;
    _array = new_array;
    _capacity = _length << 1;
}

template <typename T>
int Vector<T>::length() {
    return _length;
}

template <typename T>
int Vector<T>::capacity() {
    return _capacity;
}

template <typename T>
T Vector<T>::get(int index) {
    return _array[index];
}

template <typename T>
void Vector<T>::set(int index, T t) {
    if (_length <= index)
        _length = index + 1;

    while (_length > _capacity)
        expand();

    _array[index] = t;
}

template <typename T>
T Vector<T>::pop() {
    return _array[--_length];
}

template <typename T>
void Vector<T>::delete_index(int index) {
    for (int i = index; i + 1 < _length; i++) {
        _array[i] = _array[i+1];
    }
    _length--;
}

template <typename T>
void Vector<T>::clear() {
    _length = 0;
}

template <>
int Vector<char>::index(char t) {
    for (int i = 0; i < _length; i++) {
        if (_array[i] == t) {
            return i;
        }
    }

    return -1;
}

template class Vector<Block>;
