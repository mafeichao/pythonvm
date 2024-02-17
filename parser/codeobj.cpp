#include "codeobj.hpp"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int CodeObject::write_to_file(string& filename, CodeObject& co) {
    ofstream file = ofstream(filename, ios::out | ios::binary);
    int magic_number = 0xa0df303;

    if (!file.good()) {
        cerr << "stream buf state is bad" << endl;
        return -1;
    }

    file.write(reinterpret_cast<const char*>(&magic_number), 4);

    co.save_to_file(file);

    file.close();
    return 0;
}

int CodeObject::save_to_file(ofstream& file) {
    save_code_to_file(file);

    return 0;
}

template<typename T>
int CodeObject::save_const_value(T t, ofstream& file) {
    file.write(reinterpret_cast<const char*>(&t), sizeof(T));
    return 0;
}

int CodeObject::save_code_to_file(ofstream& file) {
    file << 's';
    save_const_value((int)_insts.size(), file);
    for (auto it = _insts.begin(); it != _insts.end(); it++) {
        file << *it;
    }
    return 0;
}

