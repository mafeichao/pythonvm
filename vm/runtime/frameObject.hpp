#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "code/codeObject.hpp"
#include "util/map.hpp"
#include "object/arrayList.hpp"

class FunctionObject;

class FrameObject {
public:
    FrameObject(CodeObject* codes);
    FrameObject(FunctionObject* func);
    ~FrameObject();

    ArrayList<HiObject*>* _stack;

    HiList* _consts;
    HiList* _names;

    Map<HiObject*, HiObject*>* _locals;

    CodeObject*           _codes;
    FrameObject*          _sender;
    int                   _pc;

public:
    void set_sender(FrameObject* x) { _sender = x; }
    FrameObject* sender()           { return _sender;}
    void set_pc(int x)              { _pc = x; }
    int  get_pc()                   { return _pc; }

    ArrayList<HiObject*>* stack()   { return _stack; }
    HiList* consts()                { return _consts; }
    HiList* names()                 { return _names; }
    Map<HiObject*, HiObject*>* locals()           { return _locals; }

    bool has_more_codes();
    unsigned char get_op_code();
    int  get_op_arg();
};

#endif

