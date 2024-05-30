#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "code/binaryFileParser.hpp"
#include "runtime/universe.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "util/handles.hpp"

BinaryFileParser::BinaryFileParser(BufferedInputStream* buf_file_stream) :
    _string_table(HiList::new_instance()), _cache(HiList::new_instance()) {
    file_stream = buf_file_stream;
    _debug_level = 0;
}

CodeObject* BinaryFileParser::parse() {
    int magic_number = file_stream->read_int();
    printf("magic number is 0x%x\n", magic_number);
    int file_flag = file_stream->read_int();

    // 打印时间戳
    char buffer[80];
    time_t moddate = (time_t)file_stream->read_int();
    struct tm * timeinfo = localtime(&moddate);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("%s\n", buffer);

    int file_size = file_stream->read_int();
    printf("size of source file is %d\n", file_size);

    char object_type = file_stream->read();
    bool ref_flag = (object_type & 0x80) != 0;
    object_type &= 0x7f;

    if (object_type == 'c') {
        // 这里需要先占位
        int index = 0;
        if (ref_flag) {
            index = _cache->length();
            _cache->append(nullptr);
        }

        Handle<CodeObject*> result = get_code_object();

        if (ref_flag) {
            _cache->set(index, result);
        }

        printf("parse OK!\n");
        return result;
    }

    return nullptr;
}

CodeObject* BinaryFileParser::get_code_object() {
    int argcount  = file_stream->read_int();
    int posonly_argcount = file_stream->read_int();
    int kwonly_argcount  = file_stream->read_int();
    int nlocals   = file_stream->read_int();
    int stacksize = file_stream->read_int();
    int flags     = file_stream->read_int();
    //printf("flags is 0x%x\n", flags);

    Handle<HiString*> byte_codes = get_byte_codes();
    Handle<HiList*> consts     = get_consts();
    Handle<HiList*> names      = get_names();
    Handle<HiList*> var_names  = get_var_names();
    Handle<HiList*> free_vars  = get_free_vars();
    Handle<HiList*> cell_vars  = get_cell_vars();

    Handle<HiString*> file_name   = get_file_name();
    Handle<HiString*> module_name = get_name();
    int begin_line_no     = file_stream->read_int();
    Handle<HiString*> lnotab      = get_no_table();

    return new CodeObject(argcount, nlocals, posonly_argcount, kwonly_argcount, stacksize, flags, 
        byte_codes, consts, names, var_names, free_vars, cell_vars, file_name, module_name,
        begin_line_no, lnotab);
}

HiString* BinaryFileParser::get_string(bool long_string) {
    unsigned int length = 0;

    if (long_string) {
        length = (unsigned int)file_stream->read_int();
    }
    else {
        length = (unsigned int)((unsigned char)file_stream->read());
    }

    char* str_value = new char[length];

    for (int i = 0; i < length; i++) {
        str_value[i] = file_stream->read();
    }

    HiString* s = HiString::new_instance(str_value, length);
    delete[] str_value;

    return s;
}

HiString* BinaryFileParser::get_name() {
    char ch = file_stream->read();
    bool ref_flag = (ch & 0x80) != 0;
    ch &= 0x7f;

    Handle<HiString*> s = nullptr;

    if (ch == 's') {
        s = get_string(true);
    }
    else if (ch == 't') {
        s = get_string(true);
    }
    else if (ch == 'z') {
        s = get_string(false);
    }
    else if (ch == 'Z') {
        s = get_string(false);
    }
    else if (ch == 'R') {
        s = _string_table->get(file_stream->read_int())->as<HiString>();
    }
    else if (ch == 'r') {
        s = _cache->get(file_stream->read_int())->as<HiString>();
    }
     
    if (ref_flag) {
        _cache->append(s);
        log(_cache->length() - 1, s);
    }

    return s;
}

HiString* BinaryFileParser::get_file_name() {
    return get_name();
}

HiString* BinaryFileParser::get_byte_codes() {
    char object_type = file_stream->read();
    bool ref_flag = (object_type & 0x80) != 0;
    assert((object_type & 0x7f) == 's');

    Handle<HiString*> s = get_string(true);

    if (ref_flag) {
        _cache->append(s);
        log(_cache->length() - 1, s);
    }

    return s;
}

HiString* BinaryFileParser::get_no_table() {
    char object_type = file_stream->read();
    bool ref_flag = (object_type & 0x80) != 0;
    char ch = object_type & 0x7f;
    Handle<HiString*> s = nullptr;

    if (ch == 's' || ch == 't') {
        s =  get_string(true);
    }
    else if (ch == 'r') {
        s = _cache->get(file_stream->read_int())->as<HiString>();
    }
    else {
        file_stream->unread();
        printf("expect a string for no table, but got %c\n", ch);
    }

    if (ref_flag) {
        _cache->append(s);
        log(_cache->length() - 1, s);
    }
    
    return s;
}

HiList* BinaryFileParser::try_to_get_tuple() {
    char obj_type = file_stream->read();
    bool ref_flag = (obj_type & 0x80) != 0;
    obj_type &= 0x7f;

    Handle<HiList*> result = nullptr;
    if (obj_type == ')') {
        int index = _cache->length();
        if (ref_flag) {
            _cache->append(nullptr);
        }

        result = get_tuple();
        
        if (ref_flag) {
            _cache->set(index, result);
            log(index, result);
        }
    }
    else if (obj_type == 'r') {
        int index = file_stream->read_int();
        result = _cache->get(index)->as<HiList>();
    }
    else {
        file_stream->unread();
        printf("unrecognized object type: %c\n", obj_type);
    }

    return result;
}

HiList* BinaryFileParser::get_consts() {
    return try_to_get_tuple();
}

HiList* BinaryFileParser::get_names() {
    return try_to_get_tuple();
}

HiList* BinaryFileParser::get_var_names() {
    return try_to_get_tuple();
}

HiList* BinaryFileParser::get_free_vars() {
    return try_to_get_tuple();
}

HiList* BinaryFileParser::get_cell_vars() {
    return try_to_get_tuple();
}

HiList* BinaryFileParser::get_tuple() {
    unsigned char length = (unsigned char)file_stream->read();
    Handle<HiList*> list = HiList::new_instance();
    int index = 0;

    for (int i = 0; i < length; i++) {
        char obj_type = file_stream->read();
        bool ref_flag = (obj_type & 0x80) != 0;
        obj_type &= 0x7f;

        Handle<HiObject*> obj = nullptr;

        // 需要先占位，最后再设置，避免递归结构出问题
        if (ref_flag) {
            index = _cache->length();
            _cache->append(nullptr);
        }

        switch (obj_type) {
        case 'c':
            //printf("got a code object\n");
            obj = get_code_object();
            break;
        case 'i':
            obj = new HiInteger(file_stream->read_int());
            break;
        case 'N':
            obj = Universe::HiNone;
            break;
        case 'T':
            obj = Universe::HiTrue;
            break;
        case 'F':
            obj = Universe::HiFalse;
            break;
        case 't':
            obj = get_string(true);
            break;
        case 'z':
        case 'Z':
            obj = get_string(false);
            break;
        case 's':
            obj = get_string(true);
            break;
        case 'R':
            obj = _string_table->get(file_stream->read_int());
            break;
        case 'r':
            obj = _cache->get(file_stream->read_int());
            break;
        case ')':
            obj = get_tuple();
            break;
        default:
            printf("parser, unrecognized type : %c\n", obj_type);
        }

        list->append(obj);
        if (ref_flag) {
            _cache->set(index, obj);
            log(index, obj);
        }
    }

    return list;
}

void BinaryFileParser::log(int index, HiObject* o) {
    if (_debug_level <= 0) {
        return;
    }

    printf("cache an object, %d : ", index);
    o->print();
    printf("\n");
}

