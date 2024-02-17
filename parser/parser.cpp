#include "parser.hpp"
#include "ast.hpp"
#include "visitor.hpp"
#include "codegen.hpp"

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

Node* Parser::expression() {
    Node* a = term();
    Token* op = get_token();
    while (op != NULL &&
        (op->_tt == T_PLUS || op->_tt == T_MINUS)) {
        consume();
        Node* b = term();
        if (op->_tt == T_PLUS) {
            a = new BinaryOp(AST_OP_ADD, a, b);
        } else {
            a = new BinaryOp(AST_OP_SUB, a, b);
        }

        op = get_token();
    }

    return a;
}

Node* Parser::term() {
    Node* a = factor();
    Token* op = get_token();
    while (op != NULL &&
        (op->_tt == T_MULT || op->_tt == T_DIV)) {
        consume();
        Node* b = factor();
        if (op->_tt == T_MULT) {
            a = new BinaryOp(AST_OP_MUL, a, b);
        } else {
            a = new BinaryOp(AST_OP_DIV, a, b);
        }

        op = get_token();
    }

    return a;
}

Node* Parser::factor() {
    Token* data = get_token();
    if (data->_tt == T_INT) {
        consume();
        return new ConstInt(stoi(data));
    }
    else if (data->_tt == T_LEFT_PAR) {
        match(T_LEFT_PAR);
        Node* a = expression();
        match(T_RIGHT_PAR);

        return a;
    }
    return NULL;
}

int Parser::stoi(Token* data) {
    int value = 0;
    for (int i = 0; i < data->_length; i++) {
        value = value * 10 + data->_value[i] - '0';
    }

    return value;
}

Node* Parser::parse() {
    return expression();
}
