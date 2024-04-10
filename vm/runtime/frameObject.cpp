#include "runtime/frameObject.hpp"
#include "object/hiString.hpp"

// this constructor is used for module only.
FrameObject::FrameObject(CodeObject* codes) {
    _consts  = codes->_consts;
    _names   = codes->_names;
    
    _locals  = new Map<HiObject*, HiObject*>();

    _stack  = new ArrayList<HiObject*>();

    _codes = codes;
    _pc    = 0;
}

int FrameObject::get_op_arg() {
    return  _codes->_bytecodes->value()[_pc++] & 0xff;
}

unsigned char FrameObject::get_op_code() {
    return _codes->_bytecodes->value()[_pc++];
}

bool FrameObject::has_more_codes() {
    return _pc < _codes->_bytecodes->length();
}
