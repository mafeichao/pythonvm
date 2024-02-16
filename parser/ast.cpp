#include "ast.hpp"
#include "lexer.hpp"
#include "visitor.hpp"

void BinaryOp::set_left(Node* left) {
    _left = left;
}

void BinaryOp::set_right(Node* right) {
    _right = right;
}


/**
 * visitor pattern for AST.
 */
void ConstInt::accept(Visitor* v) {
    v->visit(this);
}

void BinaryOp::accept(Visitor* v) {
    v->visit(this);
}
