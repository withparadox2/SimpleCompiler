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
#include <string>

using std::vector;
using std::string;

enum KIND {
    CLASS,
};

class Parser {
private:
    Lexer &L;

    bool match(Token &token);
    bool match(Token &token, string errStr);

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

    JCExpression *term(int mode);

    JCExpression *term();
    JCExpression *termRest(JCExpression *t);

    JCExpression *term1();
    JCExpression *term1Rest(JCExpression *t);

    JCExpression *term2();
    JCExpression *term2Rest(JCExpression *t, int prec);

    JCExpression *term3();

    JCExpression *basicType();

    JCExpression *bracketOpt(JCExpression *e);

    JCExpression *bracketsOptCont(JCExpression *e);

    Tree *methodDeclaratorRest(
            JCModifiers *mods,
            JCExpression *type,
            Name &name,
            bool isVoid);

    vector<JCVariableDecl*> *formalParameters();
    JCVariableDecl* formalParameter();
    JCBlock* block();

    Name &ident();

    vector<JCStatement*> *blockStatements();

    JCExpression* parExpression();
    JCStatement* parseStatement();
    vector<JCStatement*> *forInit();
    vector<JCExpressionStatement*> *forUpdate();
    vector<JCVariableDecl*> *variableDeclarators();//TODO implementation

    int prec(Token &token);

    JCExpression *literal();
    JCExpression *creator();
    JCExpression *arrayCreatorRest(JCExpression *elemtype);
    JCExpression *qualident();
    vector<JCExpression *> *arguments();
    JCMethodInvocation *arguments(JCExpression *t);
};


#endif //SIMPLECOMPILER_PARSER_H