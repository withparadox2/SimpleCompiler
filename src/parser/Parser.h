//
// Created by withparadox2 on 2016/8/23.
//

#ifndef SIMPLECOMPILER_PARSER_H
#define SIMPLECOMPILER_PARSER_H

#include "Lexer.h"
#include "Token.h"
#include <deque>
#include "../tree/tree.h"

enum KIND {
    CLASS,
};

class Parser {
private:
    Lexer &L;
    bool match(Token &token);
public:
    Parser(Lexer &lexer);
    Tree* parse();
    JCClassDecl* buildClass();
    Tree* classBodyDecl();
    JCModifiers* modifiersOpt();
    Name& indent();
};


#endif //SIMPLECOMPILER_PARSER_H