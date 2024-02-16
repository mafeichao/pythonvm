#include "visitor.hpp"
#include "ast.hpp"

void Visitor::visit(Node* n) {
    n->accept(this);
}

void Dumper::print_indent() {
    for (int i = 0; i < _indent; i++) {
        printf("  ");
    }
}

void Dumper::visit(Node* n) {
    n->accept(this);
}

void Dumper::visit(BinaryOp* op) {
    switch(op->op_type()) {
    case AST_OP_ADD:
        printf("(+ ");
        break;
    case AST_OP_SUB:
        printf("(- ");
        break;
    case AST_OP_MUL:
        printf("(* ");
        break;
    case AST_OP_DIV:
        printf("(/ ");
        break;
    default:
        printf("Unknown binary op %d\n", op->op_type());
        return;
    }

    visit(op->left());
    printf(" ");
    visit(op->right());
    printf(")");
}

void Dumper::visit(ConstInt* n) {
    printf("%d", n->_value);
}

