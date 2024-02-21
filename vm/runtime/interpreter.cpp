#include "runtime/interpreter.hpp"
#include "object/hiString.hpp"

#include "code/bytecode.hpp"

#include <string.h>
#include <stdio.h>

Interpreter::Interpreter() {
}

void Interpreter::run(HiString* codes) {
    int pc = 0;
    int code_length = codes->length();

    _stack  = new int[16];
    int top = 0;

    while (pc < code_length) {
        unsigned char op_code = codes->value()[pc++];
        bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_argument) {
            op_arg = (codes->value()[pc++] & 0xFF);
        }

        int v, w;

        switch (op_code) {
            case ByteCode::LOAD_CONST:
                _stack[top++] = op_arg;
                break;

            case ByteCode::BINARY_ADD:
                v = _stack[--top];
                w = _stack[--top];
                _stack[top++] = v + w;
                break;

            case ByteCode::BINARY_MULTIPLY:
                v = _stack[--top];
                w = _stack[--top];
                _stack[top++] = v * w;
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
    printf("%d\n", _stack[0]);

    delete[] _stack;
}
