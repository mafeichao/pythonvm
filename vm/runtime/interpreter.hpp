#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"
#include "util/handles.hpp"

class FrameObject;
class HiDict;
class OopClosure;
class ModuleObject;
class Traceback;
class Generator;

class Interpreter {
    enum Status {
        IS_OK,
        IS_EXCEPTION,
        IS_YIELD,
    };

private:
    ModuleObject*   _builtins;
    HiDict*         _modules;
    FrameObject*    _frame;
    HiObject*       _ret_value;
    HiList*         _search_path;

    HiObject*       _exception_class;
    HiObject*       _pending_exception;
    HiObject*       _trace_back;
    HiObject*       _old_exception;
    Status          _int_status;

    static Interpreter* _instance;

    Interpreter();

    void destroy_frame();

public:
    static void destroy();
    static Interpreter* get_instance();
    void initialize();

    void run(CodeObject* codes);
    HiDict* run_mod(Handle<CodeObject*> codes, Handle<HiString*> mod_name);
    HiList* search_path()   { return _search_path; }

    Status    raise_error     (const char* ename);
    Status    do_raise        (HiObject* exc, HiObject* val, HiObject* tb);
    HiObject* call_virtual    (Handle<HiObject*> func, Handle<HiList*> args);
    HiObject* eval_generator  (Generator* g);

    void build_frame(Handle<HiObject*> callable, 
        Handle<HiList*> args, Handle<HiList*> kwargs = nullptr);
    void enter_frame     (FrameObject* frame);
    void eval_frame      ();
    void leave_frame     ();
    void normalize_errors(HiObject** raw_exc, HiObject** raw_val, HiObject** raw_tb);
    void set_error_str   (HiString* name);
    void set_error_object(HiObject* raw_exc, HiObject* raw_val, HiObject* raw_tb);

    void PUSH(Handle<HiObject*> x);

    void      oops_do         (OopClosure* f);
    void add_search_path(const char* path);
};

#endif

