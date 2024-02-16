#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

class Parser {
private:
    Lexer* _lex;
    Token* _cur_token;
    
public:
    Parser(Lexer* lex);
    ~Parser();

    int eval();
    
    void consume();
    Token* get_token();
    void match(TokenType tt);
    
    int expression();
    int term();
    int factor();
    int stoi(Token* data);
};

void eval(const char* s);

#endif
