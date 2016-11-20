//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "../util/error.h"

using namespace std;

Parser::Parser(Lexer &lexer) : lexer(lexer), tokenIndex(0) {
}

ClassNode *Parser::parse() {
    return buildClassNode();
}

void Parser::printTokens() {
    while (true) {
        Token &token = takeToken();
        if (token.id == Token::END) {
            break;
        }
        cout << token.name << endl;
    }
}

Token &Parser::takeToken() {
    if (tokenIndex > 0) {
        tokenIndex--;
        Token &token = tokenQue.at(tokenIndex);
        return token;
    } else {
        tokenQue.push_front(lexer.nextToken());
        if (tokenQue.size() > MAX_DEQUE_SIZE) {
            tokenQue.pop_back();
        }
        return tokenQue.front();
    }
}

void Parser::unTakeToken() {
    tokenIndex++;
    if (tokenIndex > MAX_DEQUE_SIZE) {
        error("the max size of queue of token needs to be expanded.");
    }
}

bool Parser::match(std::string lexeme) {
    Token &token = takeToken();
    if (token.name == lexeme) {
        return true;
    }
    error("expected " + lexeme);
}


ClassNode *Parser::buildClassNode() {
    match("class");
    //do not use &nameToken, may be pop out
    Token nameToken = takeToken();
    if (nameToken.id != Token::IDENTIFIER) {
        error("expected class name");
    }
    match("{");
    FuncNode *funcNode = buildFuncNode();
    match("}");
    ClassNode *node = new ClassNode(nameToken.name);
    node->funcNodes.push_back(funcNode);
    cout << "parse success" << endl;
    return node;
}

FuncNode *Parser::buildFuncNode() {
    FuncNode *funcNode = new FuncNode;

    ModifierNode *modifierNode;
    while ((modifierNode = buildModifierNode()) != nullptr) {
        funcNode->modifierNodes.push_back(modifierNode);
    }

    TypeNode *typeNode = buildTypeNode();
    if (typeNode == nullptr) {
        error("return type can not be null");
    }

    funcNode->returnTypeNode = typeNode;

    Token &token = takeToken();
    if (token.id == Token::IDENTIFIER) {
        funcNode->name = token.name;
    } else {
        error("func name can not be null");
    }

    match("(");

    ParaNode *paraNode;
    while ((paraNode = buildParaNode()) != nullptr) {
        funcNode->parameterNodes.push_back(paraNode);
    }

    match(")");
    match("{");

    InvokeNode *statementNode;
    while ((statementNode = buildStatementNode()) != nullptr) {
        funcNode->statementNodes.push_back(statementNode);
    }

    match("}");

    return funcNode;
}

ModifierNode *Parser::buildModifierNode() {
    Token &token = takeToken();
    if (token.id == Token::IDENTIFIER) {
        if (token.name == "public") {
            return new ModifierNode(ModifierNode::PUBLIC);
        } else if (token.name == "static") {
            return new ModifierNode(ModifierNode::STATIC);
        }
    }
    unTakeToken();
    return nullptr;
}

TypeNode *Parser::buildTypeNode() {
    Token &token = takeToken();
    if (token.id == Token::IDENTIFIER) {
        if (token.name == "void") {
            return new TypeNode(TypeNode::TYPE_PRIMITIVE, token.name);
        }
        TypeNode *node = new TypeNode(TypeNode::TYPE_OBJECT, token.name);
        if (matchLight(Token::CHARLITERAL, "[")) {
            if (matchLight(Token::CHARLITERAL, "]")) {
                node->isArray = true;
            } else {
                error("array declaration need both [ and ]");
            }
        } else {
            unTakeToken();
        }
        return node;

    }
    unTakeToken();
    return nullptr;
}

ParaNode *Parser::buildParaNode() {
    TypeNode *node = buildTypeNode();
    if (node == nullptr) {
        return nullptr;
    }

    Token &token = takeToken();
    if (token.id != Token::IDENTIFIER) {
        error("parameter need a name");
    }

    return new ParaNode(node, token.name);
}

//invoke
InvokeNode *Parser::buildStatementNode() {
    SelectNode *objNode = buildSelectNode();
    if (objNode == nullptr) {
        return nullptr;
    }
    match("(");
    StringLiteralNode *expNode = buildExpressionNode();
    match(")");
    match(";");
    InvokeNode *sNode = new InvokeNode();
    sNode->selectNode = objNode;
    sNode->argNode = expNode;
    return sNode;
}

SelectNode *Parser::buildSelectNode() {
    Token *token = &takeToken();
    SelectNode *node = nullptr;
    SelectNode *n = nullptr;
    while (token->id == Token::IDENTIFIER) {
        if (node == nullptr) {
            n = new SelectNode(token->name);
            node = n;
        } else {
            n->next = new SelectNode(token->name);
            n = n->next;
        }
        token = &takeToken();
        if (token->name != ".") {
            break;
        } else {
            token = &takeToken();
        }
    }
    unTakeToken();
    return node;
}

StringLiteralNode *Parser::buildExpressionNode() {
    Token &token = takeToken();
    if (token.id == Token::STRINGLITERAL) {
        return new StringLiteralNode(token.name);
    }
    return nullptr;
}

bool Parser::matchLight(int id, std::string lexeme) {
    Token &token = takeToken();
    return token.id == id && token.name == lexeme;
}