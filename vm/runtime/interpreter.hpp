#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"

class FrameObject;
class HiDict;

class Interpreter {
private:
    HiDict*         _builtins;
    FrameObject*    _frame;
    HiObject*       _ret_value;

    static Interpreter*   _instance;
    Interpreter();

     void      destroy_frame   ();

public:
    static void destroy();
    static Interpreter* get_instance();

    void run(CodeObject* codes);
    void build_frame(HiObject* callable, HiList* args, HiList* kwargs = nullptr);
    void enter_frame     (FrameObject* frame);
    void eval_frame      ();
    void leave_frame     ();

    HiObject* call_virtual    (HiObject* func, HiList* args);
};

#endif

