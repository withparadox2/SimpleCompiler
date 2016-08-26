//
// Created by withparadox2 on 2016/8/21.
//

#ifndef SIMPLECOMPILER_WORD_H
#define SIMPLECOMPILER_WORD_H

#include "Token.h"

class Word : Token {
private:
    std::string lexeme;
public:
    Word(std::string str, int id);
    std::string getLexeme();
};


#endif //SIMPLECOMPILER_WORD_H
