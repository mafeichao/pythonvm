#ifndef MEMORY_HEAP_HPP
#define MEMORY_HEAP_HPP

#include <memory.h>

class HiObject;

class Space {
friend class Heap;
private:
    char*  _base;
    char*  _top;
    char*  _end;
    size_t _size;
    size_t _capacity;
    double _rate;

    Space(size_t size);
    ~Space();

public:
    void* allocate(size_t size);
    void clear();
    bool can_alloc(size_t size);
    bool has_obj(char* obj);
    double rate()  { return _rate; }
};

class Heap {
private:
    Space* mem_1;
    Space* mem_2;
    
    Space* eden;
    Space* survivor;

    Space* metaspace;
    bool   _during_gc;

    Heap(size_t size);

public:
    static size_t MAX_CAP;
    static Heap* instance;
    static Heap* get_instance();

    ~Heap();

    void* allocate(size_t size, bool force_gc = false);
    void* allocate_meta(size_t size);
    void copy_live_objects();
    double rate() { return eden->rate(); }

    void gc();
};

#endif

