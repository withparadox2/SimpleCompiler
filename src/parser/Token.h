//
// Created by withparadox2 on 2016/8/14.
//

#ifndef SIMPLECOMPILER_TOKEN_H
#define SIMPLECOMPILER_TOKEN_H

#include <string>
#include <map>
#include "../util/names.h"

class Token {
private:
    std::string name;

    void initToken();

    static int tokenCount;
    static std::map<Name *, Token *> nameToToken;

public:
    int id;//used in switch
    Token(std::string name);
    Token();
    std::string& desc();

    bool operator==(Token &a);
    bool operator!=(Token &a);

    static Token &fromName(Name &name);

    static Token ERROR;
    static Token _EOF;

    static Token IDENTIFIER;
    static Token BOOLEAN;
    static Token CLASS;
    static Token ELSE;
    static Token FOR;
    static Token IF;
    static Token INT;
    static Token NEW;
    static Token PRIVATE;
    static Token PUBLIC;
    static Token RETURN;
    static Token STATIC;
    static Token THIS;
    static Token VOID;

    static Token STRINGLITERAL;
    static Token INTLITERAL;
    static Token CHARLITERAL;
    static Token TRUE;
    static Token FALSE;

    static Token NULL_;
    static Token LPAREN;
    static Token RPAREN;
    static Token LBRACE;
    static Token RBRACE;
    static Token LBRACKET;
    static Token RBRACKET;

    static Token SEMI;
    static Token COMMA;
    static Token DOT;
    static Token ELLIPSIS;
    static Token EQ;
    static Token GT;
    static Token LT;
    static Token BANG;
    static Token TILDE;
    static Token QUES;
    static Token COLON;
    static Token EQEQ;
    static Token LTEQ;
    static Token GTEQ;
    static Token BANGEQ;
    static Token AMPAMP;
    static Token BARBAR;
    static Token PLUSPLUS;
    static Token SUBSUB;
    static Token PLUS;
    static Token SUB;
    static Token STAR;
    static Token SLASH;
    static Token AMP;
    static Token BAR;
    static Token CARET;
    static Token PERCENT;
    static Token LTLT;
    static Token GTGT;
    static Token GTGTGT;
    static Token PLUSEQ;
    static Token SUBEQ;
    static Token STAREQ;
    static Token SLASHEQ;
    static Token AMPEQ;
    static Token BAREQ;
    static Token CARETEQ;
    static Token PERCENTEQ;
    static Token LTLTEQ;
    static Token GTGTEQ;
    static Token GTGTGTEQ;
    static Token MONKEYS_AT;

};


#endif //SIMPLECOMPILER_TOKEN_H
