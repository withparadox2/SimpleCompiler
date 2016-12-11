//
// Created by withparadox2 on 2016/8/23.
//

#ifndef SIMPLECOMPILER_PARSER_H
#define SIMPLECOMPILER_PARSER_H

#include "Lexer.h"
#include "Token.h"
#include <deque>
#include <vector>
#include "../tree/tree.h"

using std::vector;

enum KIND {
    CLASS,
};

class Parser {
private:
    Lexer &L;

    bool match(Token &token);

    static const int EXPR = 0x1;
    static const int TYPE = 0x2;
    int mode;
    int lastMode;

    static int typeTag(Token &token);

public:
    Parser(Lexer &lexer);

    Tree *parse();

    JCClassDecl *buildClass();

    Tree *classBodyDecl(Name &className);

    JCModifiers *modifiersOpt();

    JCExpression *parseType();

    JCExpression *term();

    JCExpression *term(int mode);

    JCExpression *basicType();

    JCExpression *bracketOpt(JCExpression *e);

    JCExpression *bracketsOptCont(JCExpression *e);

    Tree *methodDeclaratorRest(
            JCModifiers *mods,
            JCExpression *type,
            Name &name,
            boolean isVoid);

    vector<JCExpression*> *formalParameters();
    JCVariableDecl* formalParameter();
    JCBlock* block();

    Name &ident();

    vector<JCStatement*> *blockStatements();

    JCExpression* parExpression();
    JCStatement* parseStatement();
};


#endif //SIMPLECOMPILER_PARSER_H