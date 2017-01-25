//
// Created by withparadox2 on 2016/12/15.
//
#ifndef SIMPLECOMPILER_PRETTY_H
#define SIMPLECOMPILER_PRETTY_H

#include "tree.h"
#include <vector>
#include <string>
#include <iostream>
#include "../tree/visitor.h"

using namespace std;

class Pretty : public Visitor {
private:
    int tabCount;

    void indent();

    void undent();

public:
    Pretty();

    void visitClassDef(JCClassDecl* that) override;

    void visitMethodDef(JCMethodDecl* that) override;

    void visitVarDef(JCVariableDecl* that) override;

    void visitBlock(JCBlock* that) override;

    void visitForLoop(JCForLoop* that) override;

    void visitIf(JCIf* that) override;

    void visitExec(JCExpressionStatement* that) override;

    void visitBreak(JCBreak* that) override;

    void visitContinue(JCContinue* that) override;

    void visitReturn(JCReturn* that) override;

    void visitApply(JCMethodInvocation* that) override;

    void visitNewClass(JCNewClass* that) override;

    void visitParens(JCParens* that) override;

    void visitAssign(JCAssign* that) override;

    void visitConditional(JCConditional* that) override;

    void visitBinary(JCBinary* that) override;

    void visitIndexed(JCArrayAccess* that) override;

    void visitSelect(JCFieldAccess* that) override;

    void visitIdent(JCIdent* that) override;

    void visitLiteral(JCLiteral* that) override;

    void visitTypeIdent(JCPrimitiveTypeTree* that) override;

    void visitTypeArray(JCArrayTypeTree* that) override;

    void visitModifiers(JCModifiers* that) override;

    void visitNewArray(JCNewArray* that) override;

    void visitTree(Tree* that) override;

    void printModifiers(JCModifiers* modifier);

    void addSpace();

    void newLine();

    void print(const string& str);

    template<typename T>
    void printTreeList(vector<T>& list, string split, string wLeft, string wRight);

    void printBrackets(JCArrayTypeTree* tree);
};


#endif //SIMPLECOMPILER_PRETTY_H
