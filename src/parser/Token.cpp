//
// Created by withparadox2 on 2016/8/14.
//
#include <iostream>
#include "Token.h"

using namespace std;


Token::Token(string name) : name(name) {
    initToken();
}

Token::Token() {
    initToken();
}


void Token::initToken() {
    id = tokenCount++;
    if (name.length() > 0) {
        Name *nameObj = Names::instance().fromString(name);
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
    return name;
}



bool Token::operator==(Token &t) {
    return t.id == this->id;
}


bool Token::operator!=(Token &t) {
    return t.id != this->id;
}

int Token::tokenCount = 0;
std::map<Name *, Token *> Token::nameToToken;


Token Token::_EOF;
Token Token::ERROR;
Token Token::IDENTIFIER;
Token Token::BOOLEAN("boolean");
Token Token::CLASS("class");
Token Token::ELSE("else");
Token Token::FOR("for");
Token Token::IF("if");
Token Token::INT("int");
Token Token::NEW("new");
Token Token::PRIVATE("private");
Token Token::PUBLIC("public");
Token Token::RETURN("return");
Token Token::STATIC("static");
Token Token::THIS("this");
Token Token::VOID("void");

Token Token::STRINGLITERAL;

Token Token::INTLITERAL;

Token Token::CHARLITERAL;

Token Token::TRUE("true");
Token Token::FALSE("false");

Token Token::NULL_("null");
Token Token::LPAREN("(");
Token Token::RPAREN(")");
Token Token::LBRACE("{");
Token Token::RBRACE("}");
Token Token::LBRACKET("[");
Token Token::RBRACKET("]");

Token Token::SEMI(";");
Token Token::COMMA(";");
Token Token::DOT(".");
Token Token::ELLIPSIS("...");
Token Token::EQ("=");
Token Token::GT(">");
Token Token::LT("<");
Token Token::BANG("!");
Token Token::TILDE("~");
Token Token::QUES("?");
Token Token::COLON(":");
Token Token::EQEQ("==");
Token Token::LTEQ("<=");
Token Token::GTEQ(">=");
Token Token::BANGEQ("!=");
Token Token::AMPAMP("&&");
Token Token::BARBAR("||");
Token Token::PLUSPLUS("++");
Token Token::SUBSUB("--");
Token Token::PLUS("+");
Token Token::SUB("-");
Token Token::STAR("*");
Token Token::SLASH("/");
Token Token::AMP("&");
Token Token::BAR("|");
Token Token::CARET("^");
Token Token::PERCENT("%");
Token Token::LTLT("<<");
Token Token::GTGT(">>");
Token Token::GTGTGT(">>>");
Token Token::PLUSEQ("+=");
Token Token::SUBEQ("-=");
Token Token::STAREQ("*=");
Token Token::SLASHEQ("/=");
Token Token::AMPEQ("&=");
Token Token::BAREQ("|=");
Token Token::CARETEQ("^=");
Token Token::PERCENTEQ("%=");
Token Token::LTLTEQ("<<=");
Token Token::GTGTEQ(">>=");
Token Token::GTGTGTEQ(">>>=");
Token Token::MONKEYS_AT("@");


