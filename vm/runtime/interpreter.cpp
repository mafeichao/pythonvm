#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "object/arrayList.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"

#include <string.h>

#define PUSH(x)  _stack->add((x))
#define POP()    _stack->pop()

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
                PUSH(_consts->get(op_arg));
                break;

            case ByteCode::LOAD_NAME:
                // "print", do nothig.
                PUSH(nullptr);
                break;

            case ByteCode::CALL_FUNCTION:
                v = POP();
                v->print();
                printf("\n");
                break;

            case ByteCode::POP_TOP:
                POP();
                break;

            case ByteCode::RETURN_VALUE:
                POP();
                break;

            case ByteCode::COMPARE_OP:
                w = POP();
                v = POP();

                switch(op_arg) {
                case ByteCode::GREATER:
                    PUSH(v->greater(w));
                    break;

                case ByteCode::LESS:
                    PUSH(v->less(w));
                    break;

                case ByteCode::EQUAL:
                    PUSH(v->equal(w));
                    break;

                case ByteCode::NOT_EQUAL:
                    PUSH(v->not_equal(w));
                    break;

                case ByteCode::GREATER_EQUAL:
                    PUSH(v->ge(w));
                    break;

                case ByteCode::LESS_EQUAL:
                    PUSH(v->le(w));
                    break;

                default:
                    printf("Error: Unrecognized compare op %d\n", op_arg);

                }
                break;

            case ByteCode::POP_JUMP_IF_FALSE:
                v = POP();
                if (v == Universe::HiFalse || v == Universe::HiNone)
                    pc = op_arg;
                break;

            case ByteCode::JUMP_FORWARD:
                pc += op_arg;
                break;


            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}
