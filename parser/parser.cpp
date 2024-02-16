a#include "parser.hpp"
#include "ast.hpp"
#include "visitor.hpp"

#include <stdlib.h>
#include <cassert>

Parser::Parser(Lexer* lex) {
    _lex = lex;
    _cur_token = NULL;
}

Parser::~Parser() {
    if (_lex) {
        delete _lex;
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

