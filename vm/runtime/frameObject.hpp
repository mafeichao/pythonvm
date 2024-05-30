#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "code/codeObject.hpp"
#include "object/arrayList.hpp"
#include "util/handles.hpp"
#include "util/vector.hpp"

class FunctionObject;
class HiDict;
class HiList;
class OopClosure;

class Block {
public:
    unsigned int  _type;
    unsigned int  _target;
    int  _level;

    Block() {
        _type = 0;
        _target = 0;
        _level = 0;
    }

    ~Block() {
        _type = 0;
        _target = 0;
        _level = 0;
    }

    Block(unsigned int b_type,
            unsigned int b_target,
            int b_level):
        _type(b_type),
        _target(b_target),
        _level(b_level) {
        }

    Block(const Block& b) {
        _type = b._type;
        _target = b._target;
        _level  = b._level;
    }
};

class FrameObject {
public:
    FrameObject();
    void initialize(Handle<CodeObject*> codes);
    void initialize(Handle<FunctionObject*> func,
        Handle<HiList*> args, Handle<HiList*> kwargs);

    ~FrameObject();

    HiList* _stack;

    HiList* _consts;
    HiList* _names;

    HiDict* _locals;
    HiDict* _globals;
    HiList* _closure;
    HiList* _fast_locals;

    BlockList* _blocks;

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

    BlockList* blocks();
    void setup_block(unsigned int btype, unsigned int target, int level);
    Block pop_block();

    HiString* file_name();
    HiString* func_name();
    int lineno();

    bool has_more_codes();
    unsigned char get_op_code();
    int  get_op_arg();
    void report_error(const char* msg, HiObject* func_name, HiObject* arg_name);

    void oops_do(OopClosure* f);
};

#endif

