//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "../util/error.h"
#include "../code/Flags.h"
#include "../code/TypeTags.h"

using namespace std;

Parser::Parser(Lexer &lexer) : L(lexer), mode(0), lastMode(0) {}

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
        type = parseType();
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

JCExpression* Parser::parseType() {
    return term(TYPE);
}

JCExpression* Parser::term(int newMode) {
    int prevmode = mode;
    mode = newMode;
    JCExpression* t = term();
    lastmode = mode;
    mode = prevmode;
    return t;
}

JCExpression* Parser::term() {
    switch (L.token()) {
        case Token::INT:
        case Token::BOOLEAN:
            return basicType();
    }
}

JCExpression* Parser::basicType() {
    JCPrimitiveTypeTree* t = new JCPrimitiveTypeTree(typeTag(L.token()));
    L.nextToken();
    return t;
}

int Parser::typeTag(Token &token) {
    switch (token.id) {
        case Token::INT:
            return TypeTags::INT;
        case Token::BOOLEAN:
            return TypeTags::BOOLEAN;
        default:
            return -1;
    }

}