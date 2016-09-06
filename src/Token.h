//
// Created by withparadox2 on 2016/8/14.
//

#ifndef SIMPLECOMPILER_TOKEN_H
#define SIMPLECOMPILER_TOKEN_H

#include <string>

class Token {
private:

public:
    int id;
    std::string lexeme;//must not be a reference
    Token(int id, std::string & text);
    Token(const Token &token);
    virtual void toString();

    static const int T_END = -1;
    static const int T_ID = 1;
    static const int T_STR = 2;
    static const int T_NUM = 3;
    static const int T_CHAR = 4;
};

#endif //SIMPLECOMPILER_TOKEN_H
