#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <stdio.h>

template <typename T>
class ArrayList {
private:
    int _capacity;
    T*  _array;
    int _length;

    void expand();

public:
    ArrayList(int n = 8);

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
};

class HiObject;
typedef ArrayList<HiObject*>* ObjList;

#endif
