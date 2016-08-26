//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "Node.h"

using namespace std;

Parser::Parser(Lexer &lexer) : lexer(lexer), tokenIndex(0) {
}

void Parser::parse() {
    Node* node = buildClassNode();
}

Token &Parser::takeToken() {
    if (tokenIndex > 0) {
        tokenIndex--;
        Token &token = tokenQue.at(tokenIndex);
        return token;
    } else {
        tokenQue.push_front(lexer.nextToken());
        if (tokenQue.size() > MAX_DEQUE_SIZE) {
            tokenQue.pop_back();
        }
        return tokenQue.front();
    }
}

void Parser::unTakeToken() {
    tokenIndex++;
    if (tokenIndex > MAX_DEQUE_SIZE) {
        //error
    }
}

bool Parser::match(std::string lexeme) {
    Token token = takeToken();
    if(token.lexeme == lexeme) {
        return true;
    }
    cerr << "expected " << lexeme;
    exit(-1);
}

void Parser::error(std::string desc) {
    cerr << desc;
    exit(-1);
}

Node* Parser::buildClassNode() {
    match("class");
    Token &nameToken = takeToken();
    if (nameToken.id != Token::T_ID) {
        error("expected class name");
    }
    match("{");
    Node* funcNode = buildFuncNode();
    match("}");
    ClassNode *node = new ClassNode(funcNode, nameToken.lexeme);
    return node;
}

Node* Parser::buildFuncNode() {

}