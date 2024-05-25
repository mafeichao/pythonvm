#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/klass.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "memory/heap.hpp"
#include "util/map.hpp"

HiString* Universe::HiTrue   = nullptr;
HiString* Universe::HiFalse  = nullptr;
HiString* Universe::HiNone   = nullptr;
Heap*     Universe::heap     = nullptr;
ArrayList<Klass*>* Universe::klasses = nullptr;

void Universe::genesis() {
    heap = Heap::get_instance();
    klasses = new ArrayList<Klass*>();

    HiTrue       = new HiString("True");
    HiFalse      = new HiString("False");
    HiNone       = new HiString("None");

    ObjectKlass::get_instance()->initialize();
    TypeKlass::get_instance()->initialize();
    IntegerKlass::get_instance()->initialize();
    DictKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();
}

void Universe::destroy() {
    delete heap;
}

