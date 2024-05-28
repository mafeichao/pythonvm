#ifndef MODULE_OBJECT_HPP
#define MODULE_OBJECT_HPP

#include "object/hiObject.hpp"
#include "util/handles.hpp"

class HiDict;

class ModuleKlass : public Klass {
private:
    static ModuleKlass* _instance;
    ModuleKlass();

public:
    static ModuleKlass* get_instance();
    void initialize();

    virtual void oops_do(OopClosure* closure, HiObject* obj);
    virtual size_t size();
};

class ModuleObject : public HiObject {
friend ModuleKlass;
private:
    HiString*   _mod_name;
 
public:
    ModuleObject(HiDict* x);
    static ModuleObject* import_module(HiObject* mod_name);
    static ModuleObject* import_so    (HiString* mod_name, HiString* file_name);
    static ModuleObject* import_pyc   (HiString* mod_name, HiString* file_name);
    static HiObject*     search_file  (Handle<HiObject*> x);

    void put(HiObject* x, HiObject* y);
    HiObject* get(HiObject* x);
};

#endif

