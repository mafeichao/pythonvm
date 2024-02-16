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

void PrintVisitor::visit(Node* n) {
    n->accept(this);
}

void PrintVisitor::visit(BinaryOp* op) {
    visit(op->left());
    visit(op->right());

    switch(op->op_type()) {
    case AST_OP_ADD:
        printf("BINARY_ADD\n");
        break;
    case AST_OP_SUB:
        printf("BINARY_SUB\n");
        break;
    case AST_OP_MUL:
        printf("BINARY_MUL\n");
        break;
    case AST_OP_DIV:
        printf("BINARY_DIV\n");
        break;
    default:
        printf("Unknown binary op %d\n", op->op_type());
        return;
    }

}

void PrintVisitor::visit(ConstInt* n) {
    printf("LOAD_CONST\t%d\n", n->_value);
}
