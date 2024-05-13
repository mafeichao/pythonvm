#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "util/map.hpp"

HiString* Universe::HiTrue   = NULL;
HiString* Universe::HiFalse  = NULL;
HiString* Universe::HiNone   = NULL;

void Universe::genesis() {
    HiTrue       = new HiString("True");
    HiFalse      = new HiString("False");
    HiNone       = new HiString("None");

    IntegerKlass::get_instance()->initialize();
    TypeKlass::get_instance()->initialize();
    DictKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();
}

void Universe::destroy() {
}

