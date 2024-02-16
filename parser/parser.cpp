#include "parser.hpp"

#include <stdlib.h>
#include <cassert>

Parser::Parser(Lexer* lex) {
    _lex = lex;
    _cur_token = NULL;
}

Parser::~Parser() {
    if (_lex) {
        _lex = NULL;
    }

    if (_cur_token) {
        delete _cur_token;
        _cur_token = NULL;
    }
}

Token* Parser::get_token() {
    if (_cur_token == NULL) {
        _cur_token = _lex->next();
        if (_cur_token)
            _cur_token->print();
    }
    
    return _cur_token;
}

void Parser::consume() {
    _cur_token = NULL;
}

void Parser::match(TokenType tt) {
    get_token();
    if (_cur_token->_tt != tt) {
        printf("Parser Error: expected %d, but got %d\n", tt, _cur_token->_tt);
        assert(_cur_token->_tt == tt); // to crash.
    }
    else {
        consume();
    }
}

int Parser::expression() {
    int a = term();
    Token* op = get_token();
    while (op != NULL &&
        (op->_tt == T_PLUS || op->_tt == T_MINUS)) {
        consume();
        int b = term();
        if (op->_tt == T_PLUS) {
            a = a + b;
        } else {
            a = a - b;
        }

        op = get_token();
    }

    return a;
}

int Parser::term() {
    int a = factor();
    Token* op = get_token();
    while (op != NULL &&
        (op->_tt == T_MULT || op->_tt == T_DIV)) {
        consume();
        int b = factor();
        if (op->_tt == T_MULT) {
            a = a * b;
        } else {
            a = a / b;
        }

        op = get_token();
    }

    return a;
}

int Parser::factor() {
    Token* data = get_token();
    if (data->_tt == T_INT) {
        consume();
        return stoi(data);
    }
    else if (data->_tt == T_LEFT_PAR) {
        match(T_LEFT_PAR);
        int a = expression();
        match(T_RIGHT_PAR);

        return a;
    }
    return -1;
}

int Parser::stoi(Token* data) {
    int value = 0;
    for (int i = 0; i < data->_length; i++) {
        value = value * 10 + data->_value[i] - '0';
    }

    return value;
}

int Parser::eval() {
    printf("%d\n", expression());

    return 0;
}
