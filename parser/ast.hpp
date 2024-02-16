#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
using namespace std;

class Token;
class Visitor;

class Node {
public:
    virtual void accept(Visitor* v) = 0;
};

enum OpType {
    AST_OP_ADD,
    AST_OP_SUB,
    AST_OP_MUL,
    AST_OP_DIV,
    AST_OP_MOD,
    AST_OP_BIT_AND,
    AST_OP_BIT_OR,
    AST_OP_BIT_XOR,
    AST_OP_LOG_AND,
    AST_OP_LOG_OR,
    AST_OP_LEFT_SHIFT,
    AST_OP_RIGHT_SHIFT,
    AST_OP_CMP,
};

class BinaryOp : public Node {
protected:
    OpType _op_type;
    Node* _left;
    Node* _right;

public:
    BinaryOp(OpType op_type, Node* left, Node* right) :
        _op_type(op_type), _left(left), _right(right) {
    }

    virtual void accept(Visitor* v);

    void set_left(Node* left);
    void set_right(Node* right);

    OpType op_type()    { return _op_type; }
    Node* left()        { return _left; }
    Node* right()       { return _right; }
};

class ConstInt : public Node {
public:
    int _value;

    ConstInt(int v) : _value(v) {}

    virtual void accept(Visitor* v);
};

#endif
