#ifndef _STACK_HPP
#define _STACK_HPP

class OopClosure;
class HiObject;

template<typename V>
class Stack {
private:
    V* vector;
    int _capacity;
    int _length;

public:
    Stack(int n = 16) {
        _capacity = n;
        vector = new V[n];
        _length = 0;
    }

    ~Stack() {
        delete[] vector;
        _capacity = 0;
        _length = 0;
    }

    void push(V v);

    V pop() {
        return vector[--_length];
    }

    V top() {
        return vector[_length - 1];
    }

    V peek(int index) {
        return vector[_length - index - 1];
    }

    int capacity() {
        return _capacity;
    }

    int length() {
        return _length;
    }

    bool empty() {
        return _length == 0;
    }

    void copy(const Stack<V>* stack);

    void oops_do(OopClosure* f);
};

template class Stack<HiObject*>;

#endif

