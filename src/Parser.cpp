//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "error.h"

using namespace std;

Parser::Parser(Lexer &lexer) : lexer(lexer), tokenIndex(0) {
}

ClassNode* Parser::parse() {
    return buildClassNode();
}

void Parser::printTokens() {
    while (true) {
        Token &token = takeToken();
        if (token.id == Token::T_END) {
            break;
        }
        cout << token.lexeme << endl;
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


ClassNode *Parser::buildClassNode() {
    match("class");
    Token &nameToken = takeToken();
    if (nameToken.id != Token::T_ID) {
        error("expected class name");
    }
    match("{");
    FuncNode *funcNode = buildFuncNode();
    match("}");
    ClassNode *node = new ClassNode(funcNode, nameToken.lexeme);
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
    if (token.id == Token::T_ID) {
        funcNode->name = token.lexeme;
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

    StatementNode *statementNode;
    while ((statementNode = buildStatementNode()) != nullptr) {
        funcNode->statementNodes.push_back(statementNode);
    }

    match("}");

    return funcNode;
}

ModifierNode *Parser::buildModifierNode() {
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

TypeNode *Parser::buildTypeNode() {
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

ParaNode *Parser::buildParaNode() {
    TypeNode *node = buildTypeNode();
    if (node == nullptr) {
        return nullptr;
    }

    Token &token = takeToken();
    if (token.id != Token::T_ID) {
        error("parameter need a name");
    }

    return new ParaNode(node, token.lexeme);
}

//invoke
StatementNode *Parser::buildStatementNode() {
    SelectNode *objNode = buildSelectNode();
    if (objNode == nullptr) {
        return nullptr;
    }
    match("(");
    ExpressionNode *expNode = buildExpressionNode();
    match(")");
    match(";");
    StatementNode *sNode = new StatementNode();
    sNode->selectNode = objNode;
    sNode->paraNode = expNode;
    return sNode;
}

SelectNode *Parser::buildSelectNode() {
    Token *token = &takeToken();
    SelectNode *node = nullptr;
    SelectNode *n = nullptr;
    while (token->id == Token::T_ID) {
        if (node == nullptr) {
            n = new SelectNode(token->lexeme);
            node = n;
        } else {
            n->next = new SelectNode(token->lexeme);
            n = n->next;
        }
        token = &takeToken();
        if (token->lexeme != ".") {
            break;
        } else {
            token = &takeToken();
        }
    }
    unTakeToken();
    return node;
}

ExpressionNode *Parser::buildExpressionNode() {
    Token &token = takeToken();
    if (token.id == Token::T_STR) {
        return new ExpressionNode(token.lexeme);
    }
    return nullptr;
}

bool Parser::matchLight(int id, std::string lexeme) {
    Token &token = takeToken();
    return token.id == id && token.lexeme == lexeme;
}