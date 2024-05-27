#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"
#include "util/handles.hpp"

class FrameObject;
class HiDict;
class OopClosure;
class ModuleObject;

class Interpreter {
private:
    ModuleObject*   _builtins;
    HiDict*         _modules;
    FrameObject*    _frame;
    HiObject*       _ret_value;

    static Interpreter* _instance;

    Interpreter();

    void destroy_frame();

public:
    static void destroy();
    static Interpreter* get_instance();
    void initialize();

    void run(CodeObject* codes);
    HiDict* run_mod(Handle<CodeObject*> codes, Handle<HiString*> mod_name);

    void build_frame(Handle<HiObject*> callable, 
        Handle<HiList*> args, Handle<HiList*> kwargs = nullptr);

    void enter_frame     (FrameObject* frame);
    void eval_frame      ();
    void leave_frame     ();

    void PUSH(Handle<HiObject*> x);

    HiObject* call_virtual    (Handle<HiObject*> func, Handle<HiList*> args);

    void      oops_do         (OopClosure* f);
};

#endif

