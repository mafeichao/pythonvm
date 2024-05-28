#include "os/shared_library.hpp"

HiObject* add(HiList* args, HiDict* kwargs) {
    int a = args->get(0)->as<HiInteger>()->value();
    int b = args->get(1)->as<HiInteger>()->value();

    return new HiInteger(a + b);
}

RGMethod math_methods[] = {
    { "add", add, 0, "add tow integer", },
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

