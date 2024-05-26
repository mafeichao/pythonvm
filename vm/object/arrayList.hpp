#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <stdio.h>

class OopClosure;

template <typename T>
class ArrayList {
private:
    int _capacity;
    T*  _array;
    int _length;

    ArrayList(int n = 8);
    void expand();

public:
    static ArrayList<T>* new_instance(int n);

    void add(T t);
    void insert(int index, T t);
    T    get(int index);
    void set(int index, T t);
    int  length();
    int  capacity();
    int  index(T t);
    T    pop();
    void clear();
    void delete_index(int index);
    void set_array(T* t) { _array = t; }

    void* operator new(size_t size);

    void  oops_do(OopClosure* f);
};

class HiObject;
typedef ArrayList<HiObject*>* ObjList;

#endif
