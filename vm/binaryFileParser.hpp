#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "bufferedInputStream.hpp"

class HiString;

class BinaryFileParser {
private:
    BufferedInputStream* file_stream;

public:
    BinaryFileParser(BufferedInputStream* stream);

public:
    HiString*   get_string();

    HiString* get_byte_codes();
};

#endif
