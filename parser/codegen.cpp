#include "codegen.hpp"
#include "ast.hpp"
#include "opcode.hpp"
#include "codeobj.hpp"

void CodeGen::visit(Node* n) {
    n->accept(this);
}

void CodeGen::add_op(unsigned char op_code, unsigned char param) {
    _insts.push_back(op_code);
    if (op_code > HAVE_ARGUMENT) {
        _insts.push_back(param);
    }
}

void CodeGen::visit(BinaryOp* op) {
    visit(op->left());
    visit(op->right());

    switch(op->op_type()) {
    case AST_OP_ADD:
        add_op(BINARY_ADD);
        break;
    case AST_OP_SUB:
        add_op(BINARY_SUB);
        break;
    case AST_OP_MUL:
        add_op(BINARY_MUL);
        break;
    case AST_OP_DIV:
        add_op(BINARY_DIV);
        break;
    default:
        printf("Unknown binary op %d\n", op->op_type());
        return;
    }
}

void CodeGen::visit(ConstInt* n) {
    add_op(LOAD_CONST, n->_value);
}

CodeObject* CodeGen::make_code_object() {
    return new CodeObject(_insts);
}


void CodeGen::dump() {
    for (int i = 0; i < _insts.size(); i++)
        printf("%d\n", _insts[i]);
}

