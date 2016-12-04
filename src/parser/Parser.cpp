//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "../util/error.h"
#include "../code/Flags.h"
#include "../code/TypeTags.h"
#include "../util/names.h"

using namespace std;

Parser::Parser(Lexer &lexer) : L(lexer), mode(0), lastMode(0) {}

Tree *Parser::parse() {
    return buildClass();
}

JCClassDecl *Parser::buildClass() {
    match(Token::CLASS);
    Name &name = ident();
    match(Token::LBRACE);

    vector<Tree *> defs;
    while (L.token() != Token::RBRACE && L.token() != Token::_EOF) {
        defs.push_back(classBodyDecl(Name & className));
    }

    match(Token::RBRACE);
}

Tree *Parser::classBodyDecl(Name &className) {
    JCModifiers *modifiers = modifiersOpt();
    JCExpression *type;
    Name *name = &L.name();
    bool isVoid = L.token() == Token::VOID;
    if (isVoid) {
        type = new JCPrimitiveTypeTree(TypeTags::VOID);
        L.nextToken();
    } else {
        type = parseType();
    }

    if (L.token() == Token::LPAREN && type->treeTag == Tree::TYPEIDENT) {
        if (*name != className) {
            errno("invalid method declaration; return type required");
        } else {
            //constructor
            return methodDeclaratorRest(modifiers, nullptr, Names::instance().init, true);
        }
    } else {
        L.nextToken();
        name = ident();
        match(Token::LPAREN);//TODO support var declaration
        return methodDeclaratorRest(modifiers, type, name, isVoid);
    }
}

Tree *Parser::methodDeclaratorRest(
        JCModifiers *mods,
        JCExpression *type,
        Name &name,
        boolean isVoid) {
    vector<JCExpression*> *params = formalParameters();
    match(Token::LBRACE);
    JCBlock *block = block();
}

JCModifiers *Parser::modifiersOpt() {
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

Name &Parser::ident() {
    if (L.token() == Token::IDENTIFIER) {
        Name &name = L.name();
        L.nextToken();
        return name;
    } else {
        match(Token::IDENTIFIER);
    }
}

JCExpression *Parser::parseType() {
    return term(TYPE);
}

JCExpression *Parser::term(int newMode) {
    int prevmode = mode;
    mode = newMode;
    JCExpression *t = term();
    lastmode = mode;
    mode = prevmode;
    return t;
}

JCExpression *Parser::term() {
    switch (L.token().id) {
        case Token::ID_INT:
        case Token::ID_BOOLEAN:
            return bracketOpt(basicType());
        case Token::ID_IDENTIFIER:
            JCExpression *t = new JCIdent(ident());
            while (true) {
                bool fail = false;
                switch (L.token().id) {
                    case Token::LBRACKET: {
                        L.nextToken();
                        if (L.token().id == Token::ID_RBRACKET) {
                            t = bracketOpt(t);
                        } else {
                            //TODO handle array access expression
                        }
                        fail = true;
                        break;
                    }
                    case Token::ID_DOT: {
                        L.nextToken();
                        t = new JCFieldAccess(t, ident());
                        break;
                    }
                    default:
                        fail = true;
                        break;
                }
                if (fail) break;
            }
            return t;
    }
    //TODO handle this
}

JCExpression *Parser::basicType() {
    JCPrimitiveTypeTree *t = new JCPrimitiveTypeTree(typeTag(L.token()));
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

JCExpression *Parser::bracketOpt(JCExpression *e) {
    if (L.token() == Token::LBRACKET) {
        L.nextToken();
        e = bracketsOptCont(e);
    }
    return e;
}

JCExpression *Parser::bracketsOptCont(JCExpression *e) {
    match(Token::RBRACKET);
    e = bracketOpt(e);
    return new JCArrayTypeTree(e);
}

vector<JCExpression*> *Parser::formalParameters() {
    vector<JCExpression*> *vec = new vector<JCExpression*>();
    match(Token::LPAREN);
    if (L.token() != Token::RPAREN) {
        vec.push_back(formalParameter());
        while (L.token() == Token::COMMA) {
            L.nextToken();
            vec.push_back(formalParameter());
        }
    }
    match(Token::RPAREN);
    return vec;
}

JCVariableDecl *Parser::formalParameter() {
    JCExpression *type = parseType();
    return new JCVariableDecl(type, ident());
}

JCBlock* block() {

}
