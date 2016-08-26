//
// Created by withparadox2 on 2016/8/21.
//

#include "Word.h"

Word::Word(std::string str, int id) : Token(id), lexeme(str) {}
std::string Word::getLexeme() {
    return lexeme;
}
