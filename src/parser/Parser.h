//
// Created by withparadox2 on 2016/8/23.
//

#ifndef SIMPLECOMPILER_PARSER_H
#define SIMPLECOMPILER_PARSER_H

#include <deque>
#include <vector>
#include <string>
#include "Lexer.h"
#include "Token.h"
#include "../tree/tree.h"


using std::vector;
using std::string;

class Parser {
private:
    Lexer& L;
    Names& names;

    bool match(Token& token);

    bool match(Token& token, string errStr);

    static const int EXPR = 0x1;
    static const int TYPE = 0x2;
    int mode;
    int lastMode;

    static int typeTag(Token& token);

    static int unoptag(Token& token);

public:
    Parser(Lexer& lexer);

    JCClassDecl* buildClass();

    Tree* classBodyDecl(Name& className);

    JCModifiers* modifiersOpt();

    JCExpression* parseType();

    JCExpression* term(int mode);

    JCExpression* term();

    JCExpression* termRest(JCExpression* t);

    JCExpression* term1();

    JCExpression* term1Rest(JCExpression* t);

    JCExpression* term2();

    JCExpression* term2Rest(JCExpression* t, int prec);

    JCExpression* term3();

    JCExpression* basicType();

    JCExpression* bracketOpt(JCExpression* e);

    JCExpression* bracketsOptCont(JCExpression* e);

    Tree* methodDeclaratorRest(
            JCModifiers* mods,
            JCExpression* type,
            Name& name,
            bool isVoid);

    JCVariableDecl::List formalParameters();

    JCVariableDecl* formalParameter();

    JCBlock* block();

    Name& ident();

    JCStatement::List blockStatements();

    JCExpression* parExpression();

    JCStatement* parseStatement();

    JCStatement::List forInit();

    JCExpressionStatement::List forUpdate();

    JCVariableDecl::List variableDeclarators();//TODO implementation

    int prec(Token& token);

    JCExpression* literal(const string& prefiex);

    JCExpression* creator();

    JCExpression* arrayCreatorRest(JCExpression* elemtype);

    JCExpression* qualident();

    JCExpression::List arguments();

    JCMethodInvocation* arguments(JCExpression* t);

    JCExpression* argumentsOpt(JCExpression* t);

    JCExpression* arrayInitializer(JCExpression* t);
};


#endif //SIMPLECOMPILER_PARSER_H