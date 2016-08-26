//
// Created by withparadox2 on 2016/8/23.
//

#ifndef SIMPLECOMPILER_LEXER_H
#define SIMPLECOMPILER_LEXER_H

#include <fstream>
#include "Token.h"

class Lexer {
private:
    std::ifstream &stream;
    int head;

    void rollBack();

    char nextChar();

    std::string sourceCode;

    Token buildToken(std::string &text, int state);

public:
    Lexer(std::ifstream &stream);

    Token nextToken();

    static const int STATE_START = 1;
    static const int STATE_COMMENT = 2;
    static const int STATE_STR = 3;
    static const int STATE_ID = 4;
    static const int STATE_CHAR = 6;
};


#endif //SIMPLECOMPILER_LEXER_H
