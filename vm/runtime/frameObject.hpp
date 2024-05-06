#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "code/codeObject.hpp"
#include "object/arrayList.hpp"

class FunctionObject;
class HiDict;

class FrameObject {
public:
    FrameObject(CodeObject* codes);
    FrameObject(FunctionObject* func, ObjList args);
    ~FrameObject() {};

    HiList* _stack;

    HiList* _consts;
    HiList* _names;

    HiDict* _locals;
    HiDict* _globals;
    ObjList                    _fast_locals;

    CodeObject*           _codes;
    FrameObject*          _sender;
    int                   _pc;

public:
    void set_sender(FrameObject* x) { _sender = x; }
    FrameObject* sender()           { return _sender;}
    void set_pc(int x)              { _pc = x; }
    int  get_pc()                   { return _pc; }

    HiList* stack()                 { return _stack; }
    HiList* consts()                { return _consts; }
    HiList* names()                 { return _names; }

    HiDict* locals()                { return _locals; }
    HiDict* globals()               { return _globals; }
    ObjList fast_locals()           { return _fast_locals; }

    bool has_more_codes();
    unsigned char get_op_code();
    int  get_op_arg();
};

#endif

