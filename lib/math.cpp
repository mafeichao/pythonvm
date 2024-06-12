#include "os/shared_library.hpp"

#include <math.h>

double get_double(HiList* args) {
    return args->get(0)->as<HiDouble>()->value();
}

HiObject* math_sqrt(HiList* args, HiDict* kwargs) {
    double x = get_double(args);
    return new HiDouble(sqrt(x));
}

HiObject* math_sin(HiList* args, HiDict* kwargs) {
    double x = get_double(args);
    return new HiDouble(sin(x));
}

RGMethod math_methods[] = {
    { "sin",  math_sin,  0, "sin(x)", },
    { "sqrt", math_sqrt, 0, "square root of x", },
    { NULL, NULL, 0, NULL, },
};

#ifdef __cplusplus
extern "C" {
#endif

SO_PUBLIC RGMethod* init_libmath() {
    return math_methods;
}

#ifdef __cplusplus
}
#endif
