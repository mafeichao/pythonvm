#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
private:
    int _capacity;
    T*  _array;
    int _length;

    void expand();

public:
    Vector(int n = 8);
    ~Vector();

    void add(T t);
    void insert(int index, T t);
    T    get(int index);
    void set(int index, T t);
    int  length();
    int  capacity();
    void delete_index(int index);
    int  index(T t);
    T    pop();
    void clear();
};

class Block;
typedef Vector<Block> BlockList;

#endif
