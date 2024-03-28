#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "util/bufferedInputStream.hpp"
#include "code/codeObject.hpp"
#include "object/arrayList.hpp"

class HiString;
class HiList;

class BinaryFileParser {
private:
    BufferedInputStream* file_stream;
    int cur;
    ArrayList<HiString*> _string_table;
    ArrayList<HiObject*> _cache;

public:
    BinaryFileParser(BufferedInputStream* stream);

public:
    CodeObject* parse();
    CodeObject* get_code_object();
    HiString*   get_byte_codes();
    HiString*   get_no_table();
    int         get_int();
    HiString*   get_string(bool long_string);
    HiString*   get_name();

    HiString*   get_file_name();

    HiList*   get_consts();
    HiList*   get_names();
    HiList*   get_var_names();
    HiList*   get_free_vars();
    HiList*   get_cell_vars();
    HiList*   get_tuple();
    HiList*   try_to_get_tuple();
};

#endif
