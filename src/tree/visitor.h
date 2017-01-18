//
// Created by withparadox2 on 2017/1/17.
//

#ifndef SIMPLECOMPILER_VISITOR_H
#define SIMPLECOMPILER_VISITOR_H

#include "tree.h"

class Visitor {
public:
    virtual void visitClassDef(JCClassDecl* that) { visitTree(that); }

    virtual void visitMethodDef(JCMethodDecl* that) { visitTree(that); }

    virtual void visitVarDef(JCVariableDecl* that) { visitTree(that); }

    virtual void visitBlock(JCBlock* that) { visitTree(that); }

    virtual void visitForLoop(JCForLoop* that) { visitTree(that); }

    virtual void visitIf(JCIf* that) { visitTree(that); }

    virtual void visitExec(JCExpressionStatement* that) { visitTree(that); }

    virtual void visitBreak(JCBreak* that) { visitTree(that); }

    virtual void visitContinue(JCContinue* that) { visitTree(that); }

    virtual void visitReturn(JCReturn* that) { visitTree(that); }

    virtual void visitApply(JCMethodInvocation* that) { visitTree(that); }

    virtual void visitNewClass(JCNewClass* that) { visitTree(that); }

    virtual void visitParens(JCParens* that) { visitTree(that); }

    virtual void visitAssign(JCAssign* that) { visitTree(that); }

    virtual void visitConditional(JCConditional* that) { visitTree(that); }

    virtual void visitBinary(JCBinary* that) { visitTree(that); }

    virtual void visitIndexed(JCArrayAccess* that) { visitTree(that); }

    virtual void visitSelect(JCFieldAccess* that) { visitTree(that); }

    virtual void visitIdent(JCIdent* that) { visitTree(that); }

    virtual void visitLiteral(JCLiteral* that) { visitTree(that); }

    virtual void visitTypeIdent(JCPrimitiveTypeTree* that) { visitTree(that); }

    virtual void visitTypeArray(JCArrayTypeTree* that) { visitTree(that); }

    virtual void visitModifiers(JCModifiers* that) { visitTree(that); }

    virtual void visitUnary(JCUnary* that) { visitTree(that); }

    virtual void visitNewArray(JCNewArray* that) { visitTree(that); }

    virtual void visitTree(Tree* that) {
        //TODO error
    }
};

class TreeScanner : public Visitor {
private:
    template <typename T>
    void scan(T& treePtr) {
        if (treePtr) {
            treePtr->accept(this);
        }
    }

    template <typename T>
    void scan(std::vector<T>& treeList) {
        for (auto iter = treeList.begin(); iter != treeList.end(); iter++) {
            scan(*iter);
        }
    }

public:

    void initTypeIfNeeded(Tree* that);

    void visitClassDef(JCClassDecl* that);

    void visitMethodDef(JCMethodDecl* that);

    void visitVarDef(JCVariableDecl* that);

    void visitBlock(JCBlock* that);

    void visitForLoop(JCForLoop* that);

    void visitIf(JCIf* that);

    void visitExec(JCExpressionStatement* that);

    void visitBreak(JCBreak* that);

    void visitContinue(JCContinue* that);

    void visitReturn(JCReturn* that);

    void visitApply(JCMethodInvocation* that);

    void visitNewClass(JCNewClass* that);

    void visitParens(JCParens* that);

    void visitAssign(JCAssign* that);

    void visitConditional(JCConditional* that);

    void visitBinary(JCBinary* that);

    void visitIndexed(JCArrayAccess* that);

    void visitSelect(JCFieldAccess* that);

    void visitIdent(JCIdent* that);

    void visitLiteral(JCLiteral* that);

    void visitTypeIdent(JCPrimitiveTypeTree* that);

    void visitTypeArray(JCArrayTypeTree* that);

    void visitModifiers(JCModifiers* that);

    void visitUnary(JCUnary* that);

    void visitNewArray(JCNewArray* that);
};

#endif //SIMPLECOMPILER_VISITOR_H
