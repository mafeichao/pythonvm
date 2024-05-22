#ifndef CODE_OBJECT_HPP
#define CODE_OBJECT_HPP

#include <vector>
#include <iostream>
#include <string>
using namespace std;

#include "codegen.hpp"

class CodeObject {
private:
    vector<unsigned char>&  _insts;

public:
    CodeObject(vector<unsigned char>& insts) : _insts(insts)
    {}

    int save_to_file(ofstream& file);
    static int write_to_file(string& filename, CodeObject& co);
    int save_code_to_file(ofstream& file);

    template<typename T>
    int save_const_value(T t, ofstream& file);
};

#endif
