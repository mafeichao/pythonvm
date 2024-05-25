#include "runtime/universe.hpp"
#include "memory/heap.hpp"
#include <cstdlib>
#include <cstdint>
#include <cstring>

using namespace std;

Heap*  Heap::instance = nullptr;
size_t Heap::MAX_CAP = 2 * 1024 * 1024;

Heap* Heap::get_instance() {
    if (instance == nullptr)
        instance = new Heap(MAX_CAP);

    return instance;
}

Heap::Heap(size_t size) {
    mem_1 = new Space(size);
    mem_2 = new Space(size);
    metaspace = new Space(size / 16);

    mem_1->clear();
    mem_2->clear();
    metaspace->clear();

    eden = mem_1;
    survivor = mem_2;
}

Heap::~Heap() {
    if (mem_1) {
        delete mem_1;
        mem_1 = nullptr;
    }

    if (mem_2) {
        delete mem_2;
        mem_2 = nullptr;
    }

    if (metaspace) {
        delete metaspace;
        metaspace = nullptr;
    }

    eden = nullptr;
    survivor = nullptr;
}

void* Heap::allocate(size_t size) {
    if (!eden->can_alloc(size)) {
        gc();
    }

    return eden->allocate(size);
}

void* Heap::allocate_meta(size_t size) {
    if (!metaspace->can_alloc(size)) {
        return nullptr;
    }

    return metaspace->allocate(size);
}

void Heap::copy_live_objects() {
}

void Heap::gc() {
}

Space::Space(size_t size) {
    _size = size;
    _base = (char*)malloc(size);
    _end  = _base + size;
    _top  = (char*)(((uintptr_t)(_base + 15)) & -16);
    _capacity = _end - _top;
}

Space::~Space() {
    if (_base) {
        free(_base);
        _base = 0;
    }

    _top = 0;
    _end = 0;
    _capacity = 0;
    _size = 0;
}

void Space::clear() {
}

void* Space::allocate(size_t size) {
    size = (size + 7) & -8;
    char* start = _top;
    _top       += size;
    _capacity  -= size;
    _rate       = _capacity * 1.0 / _size;
    //printf("after allocate %lx, _top is %p\n", size, _top);
    return start;
}

bool Space::can_alloc(size_t size) {
    return _capacity > size;
}

bool Space::has_obj(char* obj) {
    return obj >= _base && _end > obj;
}

