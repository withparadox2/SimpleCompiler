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
    Lexer &lexer;
    size_t tokenIndex;
    std::deque<Token> tokenQue;

    Token &takeToken();
    void unTakeToken();
    bool match(std::string lexeme);
    ClassNode * buildClassNode();
    FuncNode * buildFuncNode();
    ModifierNode * buildModifierNode();
    TypeNode * buildTypeNode();
    ParaNode * buildParaNode();
    InvokeNode * buildStatementNode();
    SelectNode * buildSelectNode();
    StringLiteralNode * buildExpressionNode();
    bool matchLight(int id, std::string lexeme);
public:
    const static int MAX_DEQUE_SIZE = 10;
    Parser(Lexer &lexer);
    void printTokens();
    ClassNode * parse();
};


#endif //SIMPLECOMPILER_PARSER_H