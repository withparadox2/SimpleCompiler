//
// Created by withparadox2 on 2016/8/23.
//

#ifndef SIMPLECOMPILER_LEXER_H
#define SIMPLECOMPILER_LEXER_H

#include <fstream>
#include <string>
#include "Token.h"
#include "../util/names.h"

class Lexer {
private:
    int head;
    char ch;
    Token* cToken;

    bool isSpecial(char c);

    bool isIdent(char c, bool isHead);

    bool isDigit(char c);

    void scanIdent();

    void scanOperator();

    void scanNumber();

    void nextChar();

    std::string& sourceCode;

    Names& names;

    Name* cName;


public:
    std::string bufStr;

    Lexer(std::string& sourceCode);

    void nextToken();

    Token& token();

    Name& name();

    void printTokens();
};


#endif //SIMPLECOMPILER_LEXER_H
