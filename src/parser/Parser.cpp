//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "../util/error.h"
#include "../code/Flags.h"
#include "../code/TypeTags.h"

using namespace std;

Parser::Parser(Lexer &lexer) : L(lexer) {}

Tree *Parser::parse() {
    return buildClass();
}

JCClassDecl *Parser::buildClass() {
    match(Token::CLASS);
    Name& name = indent();
    match(Token::LBRACE);

    vector<Tree*> defs;
    while (L.token() != Token::RBRACE && L.token() != Token::_EOF) {
        defs.push_back(classBodyDecl());
    }

    match(Token::RBRACE);
}

Tree* Parser::classBodyDecl() {
    JCModifiers *modifiers = modifiersOpt();
    JCExpression* type;
    bool isVoid = L.token() == Token::VOID;
    if (isVoid) {
        type = new JCPrimitiveTypeTree(TypeTags::VOID);
        L.nextToken();
    } else {

    }

}

JCModifiers* Parser::modifiersOpt() {
    long flags = 0;
    while (true) {
        long flag = 0;
        switch (L.token().id) {
            case Token::ID_PUBLIC:
                flag = Flags::PUBLIC;
                break;
            case Token::ID_STATIC:
                flag = Flags::STATIC;
                break;
            case Token::ID_PRIVATE:
                flag = Flags::PRIVATE;
                break;
        }
        if (flag == 0) {
            break;
        }

        if (flag & flags == flag) {
            errno("repeated modifier");
        }

        flags |= flag;
        L.nextToken();
    }

    return new JCModifiers(flags);
}

bool Parser::match(Token &token) {
    if (L.token() == token) {
        L.nextToken();
        return true;
    }

    error("expected " + token.fullDesc());
}

Name& Parser::indent() {
    if (L.token() == Token::IDENTIFIER) {
        Name& name = L.name();
        L.nextToken();
        return name;
    } else {
        match(Token::IDENTIFIER);
    }
}