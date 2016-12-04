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
    std::string tokenDesc;

    void initToken();

    static std::map<Name *, Token *> nameToToken;

public:
    const int id;//unique id of current token
    Token(const int id, std::string name);
    Token(const int id);

    std::string &desc();//plain text describing current token
    std::string fullDesc();
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

    //===

    const static int ID_ERROR = 0;
    const static int ID_EOF = ID_ERROR + 1;

    const static int ID_IDENTIFIER = ID_EOF + 1;
    const static int ID_BOOLEAN = ID_IDENTIFIER + 1;
    const static int ID_CLASS = ID_BOOLEAN + 1;
    const static int ID_ELSE = ID_CLASS + 1;
    const static int ID_FOR = ID_ELSE + 1;
    const static int ID_IF = ID_FOR + 1;
    const static int ID_INT = ID_IF + 1;
    const static int ID_NEW = ID_INT + 1;
    const static int ID_PRIVATE = ID_NEW + 1;
    const static int ID_PUBLIC = ID_PRIVATE + 1;
    const static int ID_RETURN = ID_PUBLIC + 1;
    const static int ID_STATIC = ID_RETURN + 1;
    const static int ID_THIS = ID_STATIC + 1;
    const static int ID_VOID = ID_THIS + 1;

    const static int ID_STRINGLITERAL = ID_VOID + 1;
    const static int ID_INTLITERAL = ID_STRINGLITERAL + 1;
    const static int ID_CHARLITERAL = ID_INTLITERAL + 1;
    const static int ID_TRUE = ID_CHARLITERAL + 1;
    const static int ID_FALSE = ID_TRUE + 1;

    const static int ID_NULL_ = ID_FALSE + 1;
    const static int ID_LPAREN = ID_NULL_ + 1;
    const static int ID_RPAREN = ID_LPAREN + 1;
    const static int ID_LBRACE = ID_RPAREN + 1;
    const static int ID_RBRACE = ID_LBRACE + 1;
    const static int ID_LBRACKET = ID_RBRACE + 1;
    const static int ID_RBRACKET = ID_LBRACKET + 1;

    const static int ID_SEMI = ID_RBRACKET + 1;
    const static int ID_COMMA = ID_SEMI + 1;
    const static int ID_DOT = ID_COMMA + 1;
    const static int ID_ELLIPSIS = ID_DOT + 1;
    const static int ID_EQ = ID_ELLIPSIS + 1;
    const static int ID_GT = ID_EQ + 1;
    const static int ID_LT = ID_GT + 1;
    const static int ID_BANG = ID_LT + 1;
    const static int ID_TILDE = ID_BANG + 1;
    const static int ID_QUES = ID_TILDE + 1;
    const static int ID_COLON = ID_QUES + 1;
    const static int ID_EQEQ = ID_COLON + 1;
    const static int ID_LTEQ = ID_EQEQ + 1;
    const static int ID_GTEQ = ID_LTEQ + 1;
    const static int ID_BANGEQ = ID_GTEQ + 1;
    const static int ID_AMPAMP = ID_BANGEQ + 1;
    const static int ID_BARBAR = ID_AMPAMP + 1;
    const static int ID_PLUSPLUS = ID_BARBAR + 1;
    const static int ID_SUBSUB = ID_PLUSPLUS + 1;
    const static int ID_PLUS = ID_SUBSUB + 1;
    const static int ID_SUB = ID_PLUS + 1;
    const static int ID_STAR = ID_SUB + 1;
    const static int ID_SLASH = ID_STAR + 1;
    const static int ID_AMP = ID_SLASH + 1;
    const static int ID_BAR = ID_AMP + 1;
    const static int ID_CARET = ID_BAR + 1;
    const static int ID_PERCENT = ID_CARET + 1;
    const static int ID_LTLT = ID_PERCENT + 1;
    const static int ID_GTGT = ID_LTLT + 1;
    const static int ID_GTGTGT = ID_GTGT + 1;
    const static int ID_PLUSEQ = ID_GTGTGT + 1;
    const static int ID_SUBEQ = ID_PLUSEQ + 1;
    const static int ID_STAREQ = ID_SUBEQ + 1;
    const static int ID_SLASHEQ = ID_STAREQ + 1;
    const static int ID_AMPEQ = ID_SLASHEQ + 1;
    const static int ID_BAREQ = ID_AMPEQ + 1;
    const static int ID_CARETEQ = ID_BAREQ + 1;
    const static int ID_PERCENTEQ = ID_CARETEQ + 1;
    const static int ID_LTLTEQ = ID_PERCENTEQ + 1;
    const static int ID_GTGTEQ = ID_LTLTEQ + 1;
    const static int ID_GTGTGTEQ = ID_GTGTEQ + 1;
    const static int ID_MONKEYS_AT = ID_GTGTGTEQ + 1;

};


#endif //SIMPLECOMPILER_TOKEN_H
