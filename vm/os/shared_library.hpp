#ifndef SHARED_LIBRARY_HPP
#define SHARED_LIBRARY_HPP

#include "runtime/functionObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiObject.hpp"

#define SO_PUBLIC __attribute__((visibility("default")))

struct RGMethod {
    const char* meth_name;
    NativeFuncPointer meth;
    int meth_info;
    const char* meth_doc;
};

typedef RGMethod* (*INIT_FUNC)();

#endif
