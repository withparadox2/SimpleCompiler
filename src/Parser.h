//
// Created by withparadox2 on 2016/8/23.
//

#ifndef SIMPLECOMPILER_PARSER_H
#define SIMPLECOMPILER_PARSER_H

#include "Lexer.h"
#include "Token.h"
#include <deque>


class Parser {
private:
    Lexer &lexer;
    int tokenIndex;
    std::deque<Token> tokenQue;
public:
    const static int MAX_DEQUE_SIZE = 10;
    Parser(Lexer &lexer);
    void parse();
    Token &takeToken();
    void unTakeToken();
};


#endif //SIMPLECOMPILER_PARSER_H
