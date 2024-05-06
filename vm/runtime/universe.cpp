#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "util/map.hpp"

HiString* Universe::HiTrue   = NULL;
HiString* Universe::HiFalse  = NULL;
HiString* Universe::HiNone   = NULL;

void Universe::genesis() {
    HiTrue       = new HiString("True");
    HiFalse      = new HiString("False");
    HiNone       = new HiString("None");

    HiDict* klass_dict = new HiDict();
    klass_dict->put(new HiString("upper"), new FunctionObject(string_upper));
    StringKlass::get_instance()->set_name(new HiString("str"));
    StringKlass::get_instance()->set_klass_dict(klass_dict);
}

void Universe::destroy() {
}

