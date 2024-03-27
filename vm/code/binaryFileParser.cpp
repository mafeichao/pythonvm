#include <stdio.h>
#include <assert.h>

#include "binaryFileParser.hpp"

#include "object/hiString.hpp"

BinaryFileParser::BinaryFileParser(BufferedInputStream* buf_file_stream) {
    file_stream = buf_file_stream;
}

CodeObject* BinaryFileParser::parse() {
    int magic_number = file_stream->read_int();
    printf("magic number is 0x%x\n", magic_number);
    int file_flag = file_stream->read_int();
    int moddate = file_stream->read_int();
    printf("moddate is 0x%x\n", moddate);
    int file_size = file_stream->read_int();
    printf("size of source file is %d\n", file_size);

    return NULL;
}

HiString* BinaryFileParser::get_string() {
    int length = file_stream->read_int();
    char* str_value = new char[length];

    for (int i = 0; i < length; i++) {
        str_value[i] = file_stream->read();
    }

    HiString* s = new HiString(str_value, length);
    delete[] str_value;

    return s;
}

HiString* BinaryFileParser::get_byte_codes() {
    assert(file_stream->read() == 's');

    return get_string();
}
