#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <map>
#include <vector>
#include <cstring>
using namespace std;

#include "visitor.hpp"
#include "opcode.hpp"

class CodeGen : public Visitor {
private:
    vector<unsigned char>   _insts;

public:
    CodeGen() {}

    vector<unsigned char>& insts() { return _insts; }

    void add_op(unsigned char op_code, unsigned char param = 0);

    void visit(Node* n);

    virtual void visit(BinaryOp* n);
    virtual void visit(ConstInt* n);

    void dump();
};

#endif
