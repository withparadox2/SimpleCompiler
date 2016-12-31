//
// Created by withparadox2 on 2016/8/14.
//
#include <iostream>
#include "Token.h"

using namespace std;


Token::Token(const int id, string name) : id(id), tokenDesc(name) {
    initToken();
}


Token::Token(const int id) : id(id){
    initToken();
}


void Token::initToken() {
    if (tokenDesc.length() > 0) {
        Name *nameObj = &Names::instance().fromString(tokenDesc);
        nameToToken.insert(make_pair(nameObj, this));
    }
}


Token &Token::fromName(Name &name) {
    if (nameToToken.find(&name) != nameToToken.end()) {
        return *nameToToken[&name];
    }
    return Token::IDENTIFIER;
}


std::string &Token::desc() {
    return tokenDesc;
}


std::string Token::fullDesc() {
    if (tokenDesc.length() > 0) return tokenDesc;
    switch (id) {
        case ID_IDENTIFIER:
            return "identifier";
    }
    return "need implementation for this token!";
}


bool Token::operator==(Token &t) {
    return &t != nullptr && t.id == this->id;
}


bool Token::operator!=(Token &t) {
    return &t == nullptr || t.id != this->id;
}

std::map<Name *, Token *> Token::nameToToken;


Token Token::_EOF(ID_EOF);
Token Token::ERROR(ID_ERROR);
Token Token::IDENTIFIER(ID_IDENTIFIER);
Token Token::BOOLEAN(ID_BOOLEAN, "boolean");
Token Token::BREAK(ID_BREAK, "break");
Token Token::CLASS(ID_CLASS, "class");
Token Token::CONTINUE(ID_CONTINUE, "continue");
Token Token::ELSE(ID_ELSE, "else");
Token Token::FOR(ID_FOR, "for");
Token Token::IF(ID_IF, "if");
Token Token::INT(ID_INT, "int");
Token Token::NEW(ID_NEW, "new");
Token Token::PRIVATE(ID_PRIVATE, "private");
Token Token::PUBLIC(ID_PUBLIC, "public");
Token Token::RETURN(ID_RETURN, "return");
Token Token::STATIC(ID_STATIC, "static");
Token Token::THIS(ID_THIS, "this");
Token Token::VOID(ID_VOID, "void");

Token Token::STRINGLITERAL(ID_STRINGLITERAL);

Token Token::INTLITERAL(ID_INTLITERAL);

Token Token::CHARLITERAL(ID_CHARLITERAL);

Token Token::TRUE(ID_TRUE, "true");
Token Token::FALSE(ID_FALSE, "false");

Token Token::NULL_(ID_NULL_, "null");
Token Token::LPAREN(ID_LPAREN, "(");
Token Token::RPAREN(ID_RPAREN, ")");
Token Token::LBRACE(ID_LBRACE, "{");
Token Token::RBRACE(ID_RBRACE, "}");
Token Token::LBRACKET(ID_LBRACKET, "[");
Token Token::RBRACKET(ID_RBRACKET, "]");

Token Token::SEMI(ID_SEMI, ";");
Token Token::COMMA(ID_COMMA, ",");
Token Token::DOT(ID_DOT, ".");
Token Token::ELLIPSIS(ID_ELLIPSIS, "...");
Token Token::EQ(ID_EQ, "=");
Token Token::GT(ID_GT, ">");
Token Token::LT(ID_LT, "<");
Token Token::BANG(ID_BANG, "!");
Token Token::TILDE(ID_TILDE, "~");
Token Token::QUES(ID_QUES, "?");
Token Token::COLON(ID_COLON, ":");
Token Token::EQEQ(ID_EQEQ, "==");
Token Token::LTEQ(ID_LTEQ, "<=");
Token Token::GTEQ(ID_GTEQ, ">=");
Token Token::BANGEQ(ID_BANGEQ, "!=");
Token Token::AMPAMP(ID_AMPAMP, "&&");
Token Token::BARBAR(ID_BARBAR, "||");
Token Token::PLUSPLUS(ID_PLUSPLUS, "++");
Token Token::SUBSUB(ID_SUBSUB, "--");
Token Token::PLUS(ID_PLUS, "+");
Token Token::SUB(ID_SUB, "-");
Token Token::STAR(ID_STAR, "*");
Token Token::SLASH(ID_SLASH, "/");
Token Token::AMP(ID_AMP, "&");
Token Token::BAR(ID_BAR, "|");
Token Token::CARET(ID_CARET, "^");
Token Token::PERCENT(ID_PERCENT, "%");
Token Token::LTLT(ID_LTLT, "<<");
Token Token::GTGT(ID_GTGT, ">>");
Token Token::GTGTGT(ID_GTGTGT, ">>>");
Token Token::PLUSEQ(ID_PLUSEQ, "+=");
Token Token::SUBEQ(ID_SUBEQ, "-=");
Token Token::STAREQ(ID_STAREQ, "*=");
Token Token::SLASHEQ(ID_SLASHEQ, "/=");
Token Token::AMPEQ(ID_AMPEQ, "&=");
Token Token::BAREQ(ID_BAREQ, "|=");
Token Token::CARETEQ(ID_CARETEQ, "^=");
Token Token::PERCENTEQ(ID_PERCENTEQ, "%=");
Token Token::LTLTEQ(ID_LTLTEQ, "<<=");
Token Token::GTGTEQ(ID_GTGTEQ, ">>=");
Token Token::GTGTGTEQ(ID_GTGTGTEQ, ">>>=");
Token Token::MONKEYS_AT(ID_MONKEYS_AT, "@");




