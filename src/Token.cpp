//
// Created by withparadox2 on 2016/8/14.
//
#include <iostream>
#include "Token.h"

using namespace std;

Token::Token(int id, string &text) : id(id), lexeme(text) {
}

Token::Token(const Token &token) : id(token.id), lexeme(token.lexeme) {
}

void Token::toString() {
    cout << lexeme << endl;
}
