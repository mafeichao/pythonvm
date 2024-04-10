#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "object/arrayList.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "util/map.hpp"

#include <string.h>

#define PUSH(x)       _frame->stack()->add((x))
#define POP()         _frame->stack()->pop()

Interpreter::Interpreter() {
}

void Interpreter::build_frame(HiObject* callable) {
    FrameObject* frame = new FrameObject((FunctionObject*) callable);
    frame->set_sender(_frame);
    _frame = frame;
}

void Interpreter::leave_frame(HiObject* return_value) {
    if (!_frame->sender()) {
        delete _frame;
        _frame = NULL;
        return;
    }

    FrameObject* temp = _frame;
    _frame         = _frame->sender();
    PUSH(return_value);

    delete temp;
}

void Interpreter::run(CodeObject* codes) {
    _frame = new FrameObject(codes);

    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        int op_arg = _frame->get_op_arg();

        FunctionObject* fo;
        HiInteger* lhs, * rhs;
        HiObject* v, * w, * u, * attr;

        switch (op_code) {
            case ByteCode::LOAD_CONST:
                PUSH(_frame->consts()->get(op_arg));
                break;

            case ByteCode::LOAD_NAME:
                v = _frame->names()->get(op_arg);
                w = _frame->locals()->get(v);
                PUSH(w);
                break;

            case ByteCode::LOAD_GLOBAL:
                v = _frame->names()->get(op_arg);
                w = _frame->locals()->get(v);
                PUSH(w);
                break;

            case ByteCode::STORE_NAME:
                v = _frame->names()->get(op_arg);
                _frame->locals()->put(v, POP());
                break;

            case ByteCode::POP_TOP:
                POP();
                break;

            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;

            case ByteCode::MAKE_FUNCTION:
                w = POP(); // function name
                v = POP();
                fo = new FunctionObject(v);
                PUSH(fo);
                break;

            case ByteCode::CALL_FUNCTION:
                if (op_arg == 1) { // print
                    v = POP();
                    v->print();
                    printf("\n");
                    POP(); // pop "nullptr"
                    PUSH(Universe::HiNone); // return value of print
                    break;
                }
                    
                build_frame(POP());
                break;

            case ByteCode::RETURN_VALUE:
                leave_frame(POP());
                if (!_frame)
                    return;
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
                    _frame->set_pc(op_arg);
                break;

            case ByteCode::JUMP_FORWARD:
                _frame->set_pc(op_arg + _frame->get_pc());
                break;

            case ByteCode::JUMP_ABSOLUTE:
                _frame->set_pc(op_arg);
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}
