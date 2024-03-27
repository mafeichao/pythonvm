#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "util/bufferedInputStream.hpp"
#include "code/codeObject.hpp"

class HiString;

class BinaryFileParser {
private:
    BufferedInputStream* file_stream;

public:
    BinaryFileParser(BufferedInputStream* stream);

public:
    CodeObject* parse();

    HiString*   get_string();

    HiString* get_byte_codes();
};

#endif
