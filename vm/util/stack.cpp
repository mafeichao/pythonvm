#include "util/stack.hpp"
#include "runtime/frameObject.hpp"
#include "memory/oopClosure.hpp"
#include <cassert>
using namespace std;

template<>
void Stack<HiObject*>::oops_do(OopClosure* f) {
    for (int i = 0; i < _capacity; i++) {
        f->do_oop(&vector[i]);
    }
}

template<typename T>
void Stack<T>::copy(const Stack<T>* stack) {
    _length = stack->_length;
    _capacity  = stack->_capacity;
    for (int i = 0; i < _length; i++) {
        vector[i] = stack->vector[i];
    }
}

template<typename T>
void Stack<T>::push(T t) {
    if (_length >= _capacity) {
        printf("gc stack overflow\n");
        assert(false);
    }

    vector[_length++] = t;
}

