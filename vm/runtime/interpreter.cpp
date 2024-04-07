#include "runtime/interpreter.hpp"
#include "object/arrayList.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"

#include <string.h>

Interpreter::Interpreter() {
}

void Interpreter::run(CodeObject* codes) {
    int pc = 0;
    int code_length = codes->_bytecodes->length();

    _stack  = new ArrayList<HiObject*>(codes->_stack_size);
    _consts = codes->_consts;

    while (pc < code_length) {
        unsigned char op_code = codes->_bytecodes->value()[pc++];
        short op_arg = (codes->_bytecodes->value()[pc++] & 0xFF);

        HiInteger* lhs, * rhs;
        HiObject* v, * w, * u, * attr;

        switch (op_code) {
            case ByteCode::LOAD_CONST:
                _stack->add(_consts->get(op_arg));
                break;

            case ByteCode::LOAD_NAME:
                // "print", do nothig.
                _stack->add(nullptr);
                break;

            case ByteCode::CALL_FUNCTION:
                v = _stack->pop();
                v->print();
                printf("\n");
                break;

            case ByteCode::POP_TOP:
                _stack->pop();
                break;

            case ByteCode::RETURN_VALUE:
                _stack->pop(); 
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}
