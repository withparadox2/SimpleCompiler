//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "Node.h"

using namespace std;

Parser::Parser(Lexer &lexer) : lexer(lexer), tokenIndex(0) {
}

void Parser::parse() {
    Node *node = buildClassNode();
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
        //error
    }
}

bool Parser::match(std::string lexeme) {
    Token &token = takeToken();
    if (token.lexeme == lexeme) {
        return true;
    }
    cerr << "expected " << lexeme;
    exit(-1);
}

void Parser::error(std::string desc) {
    cerr << desc;
    exit(-1);
}

Node *Parser::buildClassNode() {
    match("class");
    Token &nameToken = takeToken();
    if (nameToken.id != Token::T_ID) {
        error("expected class name");
    }
    match("{");
    Node *funcNode = buildFuncNode();
    match("}");
    ClassNode *node = new ClassNode(funcNode, nameToken.lexeme);
    return node;
}

Node *Parser::buildFuncNode() {
    Node *node;
    FuncNode *funcNode = new FuncNode;
    while ((node = buildModifierNode()) != nullptr) {
        funcNode->modifierNodes.push_back(node);
    }

    node = buildTypeNode();
    if (node == nullptr) {
        error("return type can not be null");
    }

    Token &token = takeToken();
    if (token.id == Token::T_ID) {
        funcNode->name = token.lexeme;
    } else {
        error("func name can not be null");
    }

    match("(");

    while ((node = buildParaNode()) != nullptr) {
        funcNode->parameterNodes.push_back(node);
    }

    match(")");
    match("{");

    while ((node = buildStatementNode()) != nullptr) {
        funcNode->statementNodes.push_back(node);
    }

    match("}");
    cout << "parse success" << endl;
}

Node *Parser::buildModifierNode() {
    Token &token = takeToken();
    if (token.id == Token::T_ID) {
        if (token.lexeme == "public") {
            return new ModifierNode(ModifierNode::PUBLIC);
        } else if (token.lexeme == "static") {
            return new ModifierNode(ModifierNode::STATIC);
        }
    }
    unTakeToken();
    return nullptr;
}

Node *Parser::buildTypeNode() {
    Token &token = takeToken();
    if (token.id == Token::T_ID) {
        if (token.lexeme == "void") {
            return new TypeNode(TypeNode::TYPE_PRIMITIVE, token.lexeme);
        }
        TypeNode *node = new TypeNode(TypeNode::TYPE_OBJECT, token.lexeme);
        if (matchLight(Token::T_CHAR, "[")) {
            if (matchLight(Token::T_CHAR, "]")) {
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

Node *Parser::buildParaNode() {
    Node *node = buildTypeNode();
    if (node == nullptr) {
        return nullptr;
    }

    Token &token = takeToken();
    if (token.id != Token::T_ID) {
        error("parameter need a name");
    }

    return new ParaNode(node, token.lexeme);
}

Node *Parser::buildStatementNode() {
    while (true) {
        Token &token = takeToken();
        if (token.id == Token::T_END || token.lexeme == ";") {
            break;
        }
    }
    return nullptr;
}

bool Parser::matchLight(int id, std::string lexeme) {
    Token &token = takeToken();
    return token.id == id && token.lexeme == lexeme;
}