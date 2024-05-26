#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "code/codeObject.hpp"
#include "object/arrayList.hpp"
#include "util/handles.hpp"

class FunctionObject;
class HiDict;
class HiList;
class OopClosure;

class FrameObject {
public:
    FrameObject();
    void initialize(Handle<CodeObject*> codes);
    void initialize(Handle<FunctionObject*> func,
        Handle<HiList*> args, Handle<HiList*> kwargs);

    ~FrameObject() {};

    HiList* _stack;

    HiList* _consts;
    HiList* _names;

    HiDict* _locals;
    HiDict* _globals;
    HiList*               _closure;
    HiList*               _fast_locals;

    CodeObject*           _codes;
    FrameObject*          _sender;
    int                   _pc;
    bool                  _entry_frame;

public:
    void set_sender(FrameObject* x) { _sender = x; }
    FrameObject* sender()           { return _sender;}
    void set_pc(int x)              { _pc = x; }
    int  get_pc()                   { return _pc; }
    void set_entry_frame(bool x)    { _entry_frame = x; }
    bool is_entry_frame()           { return _entry_frame; }
    bool is_first_frame()           { return _sender == nullptr; }

    HiList* stack()                 { return _stack; }
    HiList* consts()                { return _consts; }
    HiList* names()                 { return _names; }

    HiDict* locals()                { return _locals; }
    HiDict* globals()               { return _globals; }
    HiList* fast_locals()           { return _fast_locals; }

    HiList* closure()               { return _closure; }
    HiObject* get_cell_from_parameter(int i );

    bool has_more_codes();
    unsigned char get_op_code();
    int  get_op_arg();
    void report_error(const char* msg, HiObject* func_name, HiObject* arg_name);

    void oops_do(OopClosure* f);
};

#endif

