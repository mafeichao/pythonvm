#ifndef UTIL_MAP_HPP
#define UTIL_MAP_HPP

#include <cstdio>

using namespace std;

class OopClosure;

template <typename K, typename V>
class MapEntry {
public:
    K _k;
    V _v;
    
    MapEntry(const MapEntry<K, V>& entry);
    MapEntry(K k, V v) : _k(k), _v(v) {}
    MapEntry() : _k(0), _v(0) {}
};

template <typename K, typename V>
class Map {
private:
    MapEntry<K, V>* _entries;
    int _length;
    int _capacity;
    
    void expand();
    Map();

public:
    static Map<K, V>* new_instance(int n);
    int  length() { return _length; }
    void put(K k, V v);
    V    get(K k);
    K    get_key(int index);
    V    get_value(int index);
    V    remove(K k);
    bool has_key(K k);
    int  index(K k);
    MapEntry<K, V>* entries() { return _entries; }

    void* operator new(size_t size);
    void oops_do(OopClosure* closure);
};

#endif

