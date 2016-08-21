//
// Created by withparadox2 on 2016/8/14.
//
#include <iostream>
#include "Token.h"

Token::Token(int id) : id(id) {}

void Token::toString() {
    std::cout << "id = " << id << std::endl;
}
