#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

class Node;

class Parser {
private:
    Lexer* _lex;
    Token* _cur_token;
    
public:
    Parser(Lexer* lex);
    ~Parser();

    void eval();
    
    void consume();
    Token* get_token();
    void match(TokenType tt);
    
    Node* expression();
    Node* term();
    Node* factor();
    int stoi(Token* data);
};

void eval(const char* s);

#endif
