#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiObject.hpp"
#include "util/map.hpp"

HiInteger* Universe::HiTrue   = NULL;
HiInteger* Universe::HiFalse  = NULL;
HiObject*  Universe::HiNone   = NULL;

void Universe::genesis() {
    HiTrue       = new HiInteger(1);
    HiFalse      = new HiInteger(0);
    HiNone       = new HiObject();

    HiDict* klass_dict = new HiDict();
    klass_dict->put(new HiString("upper"), new FunctionObject(string_upper));
    StringKlass::get_instance()->set_klass_dict(klass_dict);
}

void Universe::destroy() {
}

