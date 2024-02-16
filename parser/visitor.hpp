#ifndef VISITOR_HPP
#define VISITOR_HPP

class Node;
  class BinaryOp;
  class ConstInt;

class Visitor {
public:
    void visit(Node* n);

    virtual void visit(BinaryOp* n) = 0;
    virtual void visit(ConstInt* n) = 0;
};

class Dumper : public Visitor {
private:
    int _indent;
    inline void print_indent();
public:
    Dumper() : _indent(0) {}

    void visit(Node* n);

    virtual void visit(BinaryOp* n);
    virtual void visit(ConstInt* n);
};

class PrintVisitor : public Visitor {
public:
    PrintVisitor() {}

    void visit(Node* n);

    virtual void visit(BinaryOp* n);
    virtual void visit(ConstInt* n);
};

#endif
