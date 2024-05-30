#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/typeObject.hpp"
#include "runtime/module.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "util/bufferedInputStream.hpp"
#include "util/handles.hpp"
#include "memory/oopClosure.hpp"
#include "code/binaryFileParser.hpp"
#include "os/shared_library.hpp"

#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

ModuleKlass* ModuleKlass::_instance = NULL;

ModuleKlass* ModuleKlass::get_instance() {
    if (_instance == NULL)
        _instance = new ModuleKlass();

    return _instance;
}

ModuleKlass::ModuleKlass() {
}

void ModuleKlass::initialize() {
    HiDict* dict = HiDict::new_instance();
    set_klass_dict(dict);
    set_name(HiString::new_instance("module"));
    (new HiTypeObject())->set_own_klass(this);

    add_super(ObjectKlass::get_instance());
    order_supers();
}

size_t ModuleKlass::size() {
    return sizeof(ModuleObject);
}

void ModuleKlass::oops_do(OopClosure* f, HiObject* obj) {
    f->do_oop((HiObject**)&obj->as<ModuleObject>()->_mod_name);
}

ModuleObject::ModuleObject(HiDict* dict) {
    set_obj_dict(dict);
    set_klass(ModuleKlass::get_instance());
}

HiObject* ModuleObject::search_file(Handle<HiObject*> x) {
    Handle<HiList*> paths = Interpreter::get_instance()->search_path();
    for (int i = 0; i < paths->length(); i++) {
        Handle<HiString*> path = paths->get(i)->as<HiString>(); 
        path = path->add(x)->as<HiString>();

        Handle<HiString*> name = path->add(ST(pyc))->as<HiString>();
        name->print();
        printf("\n");
        if (access(name->value(), R_OK) == 0) {
            return import_pyc(x->as<HiString>(), name);
        }

        name = path->add(ST(so))->as<HiString>();
        name->print();
        printf("\n");
        if (access(name->value(), R_OK) == 0) {
            return import_so(x->as<HiString>(), name);
        }
    }

    return Universe::HiNone;
}

ModuleObject* ModuleObject::import_pyc(HiString* raw_mod_name, HiString* file_name) {
    BufferedInputStream stream(file_name->value());
    BinaryFileParser parser(&stream);

    Handle<HiString*> mod_name(raw_mod_name);
    Handle<CodeObject*> mod_code = parser.parse();
    Handle<HiDict*> mod_dict = Interpreter::get_instance()->run_mod(mod_code, mod_name);
    return new ModuleObject(mod_dict);
}

ModuleObject* ModuleObject::import_so(HiString* mod_name, HiString* filename) {
    char* error_msg = nullptr;

    void* handle = dlopen(filename->value(), RTLD_NOW);
    if (handle == nullptr) {
        printf("error to open file: %s\n", dlerror());
        return nullptr;
    }

    char* init_meth = new char[5 + mod_name->length() + 1];
    memcpy(init_meth, "init_", 5);
    memcpy(init_meth + 5, mod_name->value(), mod_name->length());
    init_meth[5 + mod_name->length()] = '\0';

    INIT_FUNC init_func = (INIT_FUNC)dlsym(handle, init_meth);
    delete[] init_meth;

    if ((error_msg = dlerror()) != NULL) {
        printf("Symbol init_methods not found: %s\n", error_msg);
        dlclose(handle);
        return NULL;
    }

    RGMethod* ml = init_func();
    Handle<ModuleObject*> mod = new ModuleObject(nullptr);
    mod->set_obj_dict(HiDict::new_instance());

    for (; ml->meth_name != NULL; ml++) {
        Handle<HiString*> name = HiString::new_instance(ml->meth_name);
        Handle<FunctionObject*> func = new FunctionObject(ml->meth, name);
        mod->put(name, func());
    }

    return mod;
}

ModuleObject* ModuleObject::import_module(HiObject* x) {
    Handle<HiObject*> mod_name(x);
    HiObject* mod = search_file(x);

    if (mod == Universe::HiNone) {
        printf("No module named ");
        mod_name->print();
        printf("\n");
        assert(false);
        return nullptr;
    }

    return mod->as<ModuleObject>();
}

void ModuleObject::put(HiObject* x, HiObject* y) {
    obj_dict()->put(x, y);
}

HiObject* ModuleObject::get(HiObject* x) {
    return obj_dict()->get(x);
}

void ModuleObject::extend(ModuleObject* mo) {
    obj_dict()->update(mo->obj_dict());
}

