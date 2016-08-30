//
// Created by withparadox2 on 2016/8/23.
//

#ifndef SIMPLECOMPILER_PARSER_H
#define SIMPLECOMPILER_PARSER_H

#include "Lexer.h"
#include "Token.h"
#include <deque>
#include "Node.h"

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
    Node* buildClassNode();
    Node* buildFuncNode();
    Node* buildModifierNode();
    Node* buildTypeNode();
    Node* buildParaNode();
    Node* buildExpressionNode();
    Node* buildStatementNode();
    Node* buildSelectNode();
    Node* buildStrNode();
    bool matchLight(int id, std::string lexeme);
public:
    const static int MAX_DEQUE_SIZE = 10;
    Parser(Lexer &lexer);
    void parse();
    void error(std::string desc);

};


#endif //SIMPLECOMPILER_PARSER_H
