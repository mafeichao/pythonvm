#include "lexer.hpp"
#include <stdlib.h>

Token::Token(TokenType tt, const char* v, int len) {
    _tt = tt;
    _value = v;
    _length = len;
}

Lexer::Lexer(const char* filename) {
    _s = new char[BUF_LEN];
    _file = fopen(filename, "r");
    if (_file == NULL) {
        printf("Can not open file %s\n", filename);
        exit(-1);
    }

    fgets(_s, BUF_LEN, _file);
    _length = strlen(_s);
    _state = INIT;
    _index = 0;
}

Lexer::~Lexer() {
    if (_file) {
        fclose(_file);
        _file = NULL;
    }

    if (_s) {
        delete[] _s;
        _s = NULL;
    }
}

char Lexer::getchar() {
    return _s[_index++];
}

void Lexer::ungetc() {
    _index--;
}

Token* Lexer::next() {
    int start   = _index;
    int length  = 0;
    _state = INIT;

    while (_index < _length) {
        char c = getchar();
        if (_state == INIT) {
            if (c == ' ' || c == '\t') {
                start += 1;
                continue;
            }

            if (c == '\n') {
                if (!fgets(_s, BUF_LEN, _file)) {
                    return NULL;
                }
                _index = 0;
                start = 0;
                _length = strlen(_s);
                continue;
            }
            
            if (c >= '0' && c <= '9') {
                _state = INTEGER;
            }
            else if ((c >= 'a' && c <= 'z') 
                || (c >= 'A' && c <= 'Z') 
                || c == '_') {
                _state = NAME;
            }
            else if (c == '=') {
                _state = OP_EQ;
            }
            else if (c == '#') {
                return new Token(T_LAMBDA, _s + start, 1);
            }
            else if (c == '(') {
                return new Token(T_LEFT_PAR, _s + start, 1);
            }
            else if (c == ')') {
                return new Token(T_RIGHT_PAR, _s + start, 1);
            }
            else if (c == '{') {
                return new Token(T_LEFT_BRACE, _s + start, 1);
            }
            else if (c == '}') {
                return new Token(T_RIGHT_BRACE, _s + start, 1);
            }
            else if (c == ':') {
                return new Token(T_COLON, _s + start, 1);
            }
            else if (c == '+') {
                _state = OP_ADD;
            }
            else if (c == '-') {
                _state = OP_SUB;
            }
            else if (c == '*') {
                _state = OP_MUL;
            }
            else if (c == '/') {
                _state = OP_DIV;
            }
            else if (c == '<') {
                _state = OP_LT;
            }
            else if (c == '>') {
                _state = OP_GT;
            }
            else if (c == '^') {
                _state = OP_XOR;
            }
            else if (c == '&') {
                _state = OP_AND;
            }
            else if (c == '|') {
                _state = OP_OR;
            }
            else if (c == ';') {
                return new Token(T_ENDL, _s + start, 1);
            }
            
            length = 1;
        }
        else if (_state == INTEGER) {
            if (c >= '0' && c <= '9') {
                length += 1;
            }
            else {
                ungetc();
                return new Token(T_INT, _s + start, length);
            }
        }
        else if (_state == NAME) {
            if ((c >= 'a' && c <= 'z') 
                || (c >= 'A' && c <= 'Z') 
                || c == '_'
                || (c >= '0' && c <= '9')) {
                    length += 1;
            }
            else {
                ungetc();
                return filter_keyword(new Token(T_NAME, _s + start, length));
            }
        }
        else if (_state == OP_EQ) {
            if (c == '>') {
                length += 1;
                return new Token(T_LAM_DEF, _s + start, length);
            }
            else if (c == '=') {
                length += 1;
                return new Token(T_EQU, _s + start, length);
            }
            else {
                ungetc();
                return new Token(T_ASN, _s + start, length);
            }
        }
        else if (_state == OP_SLASH) {
            return new Token(T_ASN, _s + start, length);
        }
        else if (_state == OP_ADD) {
            if (c == '=') {
                
            }
            else {
                ungetc();
                return new Token(T_PLUS, _s + start, length);
            }
        }
        else if (_state == OP_SUB) {
            if (c == '=') {
                
            }
            else {
                ungetc();
                return new Token(T_MINUS, _s + start, length);
            }
        }
        else if (_state == OP_MUL) {
            if (c == '=') {
                
            }
            else {
                ungetc();
                return new Token(T_MULT, _s + start, length);
            }
        }
        else if (_state == OP_DIV) {
            if (c == '=') {
                
            }
            else {
                ungetc();
                return new Token(T_DIV, _s + start, length);
            }
        }
        else if (_state == OP_XOR) {
            if (c == '=') {
                
            }
            else {
                ungetc();
                return new Token(BIT_XOR, _s + start, length);
            }
        }
        else if (_state == OP_AND) {
            if (c == '=') {
                
            }
            else {
                ungetc();
                return new Token(BIT_AND, _s + start, length);
            }
        }
        else if (_state == OP_OR) {
            if (c == '=') {
                
            }
            else {
                ungetc();
                return new Token(BIT_OR, _s + start, length);
            }
        }
        else if (_state == OP_LT) {
            if (c == '=') {
                length += 1;
                return new Token(T_LE, _s + start, length);
            }
            else if (c == '<') {
                length += 1;
                return new Token(LEFT_SHIFT, _s + start, length);
            }
            else {
                ungetc();
                return new Token(T_LT, _s + start, length);
            }
        }
        else if (_state == OP_GT) {
            if (c == '=') {
                length += 1;
                return new Token(T_GE, _s + start, length);
            }
            else if (c == '>') {
                length += 1;
                return new Token(RIGHT_SHIFT, _s + start, length);
            }
            else {
                ungetc();
                return new Token(T_GT, _s + start, length);
            }
        }
    }
    
    return NULL;
}

Token* Lexer::filter_keyword(Token* t) {
    if (t->equals("if")) {
        t->_tt = T_KW_IF;
    }
    else if (t->equals("while")) {
        t->_tt = T_KW_WHILE;
    }
    else if (t->equals("else")) {
        t->_tt = T_KW_ELSE;
    }
    else if (t->equals("print")) {
        t->_tt = T_KW_PRINT;
    }
    else if (t->equals("or")) {
        t->_tt = LOGIC_OR;
    }
    else if (t->equals("and")) {
        t->_tt = LOGIC_AND;
    }
    else if (t->equals("not")) {
        t->_tt = LOGIC_NOT;
    }
    else if (t->equals("var")) {
        t->_tt = T_KW_VAR;
    }
    else if (t->equals("int")) {
        t->_tt = T_TYPE_INT;
    }

    return t;
}

bool Token::equals(const char* s) {
    if (_length != strlen(s)) {
        return false;
    }

    for (int i = 0; i < _length; i++) {
        if (*(_value + i) != *(s + i)) {
            return false;
        }
    }

    return true;
}

void Token::print() {
    printf("%d: ", _tt);
    for (int i = 0; i < _length; i++) {
        printf("%c", _value[i]);
    }
    printf("\n");
}

