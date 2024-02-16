#ifndef LEXER_HPP
#define LEXER_HPP

#include <stdio.h>
#include <string.h>

enum TokenType {
    T_INT,
    T_NAME,
    
    T_LAMBDA,
    T_LAM_DEF,
    T_ENDL,
    T_ASN,
    
    T_LEFT_PAR,
    T_RIGHT_PAR,
    T_LEFT_BRACE,
    T_RIGHT_BRACE,
    T_COLON,
    
    T_PLUS,
    T_MINUS,
    T_DIV,
    T_MULT,

    T_KW_IF,
    T_KW_ELSE,
    T_KW_WHILE,
    T_KW_PRINT,
    T_KW_AND,
    T_KW_OR,

    T_EQU,
    T_LT,
    T_LE,
    T_GT,
    T_GE,

    LOGIC_OR,
    LOGIC_NOT,
    LOGIC_AND,
    BIT_OR,
    BIT_XOR,
    BIT_AND,
    LEFT_SHIFT,
    RIGHT_SHIFT,

    /* About Types. */
    T_KW_VAR,
    T_TYPE_INT,
};

class Token {
public:
    TokenType   _tt;
    const char* _value;
    int         _length;
    
    Token(TokenType tt, const char* v, int length);
    bool equals(const char* s);
    
    void print();
};

enum State {
    INIT,
    INTEGER,
    NAME,
    OP_SLASH,
    
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    OP_AND,
    OP_OR,
    OP_XOR,

    OP_EQ,
    OP_LT,
    OP_GT,
    
    OP_COLON
};

class Lexer {
private:
    static const int BUF_LEN = 1024;
    char*   _s;
    int     _index;
    State   _state;
    int     _length;
    FILE*   _file;
    
public:
    Lexer(const char* filename);
    ~Lexer();
    
    Token* next();
    
    inline char   getchar();
    inline void   ungetc();
    Token* filter_keyword(Token* t);
};

#endif

