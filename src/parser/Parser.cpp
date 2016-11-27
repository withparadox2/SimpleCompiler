//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "../util/error.h"

using namespace std;

Parser::Parser(Lexer &lexer) : L(lexer) {
}

Tree *Parser::parse() {
    return buildClass();
}

JCClassDecl *Parser::buildClass() {
    match(Token::CLASS);

}


ClassNode *Parser::buildClassNode() {
    match(Token::CLASS);
    //do not use &nameToken, may be pop out
    Token nameToken = takeToken();
    if (nameToken.id != Token::IDENTIFIER) {
        error("expected class name");
    }
    match("{");
    FuncNode *funcNode = buildFuncNode();
    match("}");
    ClassNode *node = new ClassNode(nameToken.name);
    node->funcNodes.push_back(funcNode);
    cout << "parse success" << endl;
    return node;
}



bool Parser::match(Token &token) {
    if (L.token() == token) {
        L.nextToken();
        return true;
    }
    error("expected " + lexeme);
}

Name& Parser::indent() {
    if (L.token() == Token::IDENTIFIER) {
        Name& name = L.name();
        L.nextToken();
        return name;
    } else {
        match(Token::IDENTIFIER);
    }
}