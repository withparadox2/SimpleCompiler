//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"

using namespace std;

Parser::Parser(Lexer &lexer) : lexer(lexer), tokenIndex(0) {
}

void Parser::parse() {
    while (true) {
        Token &token = takeToken();
        if (token.id == Token::T_END) {
            break;
        }
        cout << token.lexeme << endl;
    }
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

