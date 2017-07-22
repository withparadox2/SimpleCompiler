//
// Created by withparadox2 on 2017/1/17.
//
#include "visitor.h"
#include "code/Symtab.h"

void TreeScanner::initTypeIfNeeded(Tree* that) {
    if (!that->type) {
        that->type = Symtab::instance().unknownType;
    }
}

void TreeScanner::visitClassDef(JCClassDecl* that) {
    scan(that->mods);
    scan(that->defs);
}

void TreeScanner::visitMethodDef(JCMethodDecl* that) {
    scan(that->mods);
    scan(that->resType);
    scan(that->params);
    scan(that->body);
}

void TreeScanner::visitVarDef(JCVariableDecl* that) {
    scan(that->vartype);
    scan(that->init);
}

void TreeScanner::visitBlock(JCBlock* that) {
    scan(that->stats);
}

void TreeScanner::visitForLoop(JCForLoop* that) {
    scan(that->init);
    scan(that->cond);
    scan(that->step);
    scan(that->body);
}

void TreeScanner::visitIf(JCIf* that) {
    scan(that->cond);
    scan(that->thenPart);
    scan(that->elsePart);
}

void TreeScanner::visitExec(JCExpressionStatement* that) {
    scan(that->exp);
}

void TreeScanner::visitBreak(JCBreak* that) {}

void TreeScanner::visitContinue(JCContinue* that) {}

void TreeScanner::visitReturn(JCReturn* that) {
    scan(that->expr);
}

void TreeScanner::visitApply(JCMethodInvocation* that) {
    scan(that->meth);
    scan(that->args);
}

void TreeScanner::visitNewClass(JCNewClass* that) {
    scan(that->clazz);
    scan(that->arguments);
}

void TreeScanner::visitParens(JCParens* that) {
    scan(that->expr);
}

void TreeScanner::visitAssign(JCAssign* that) {
    scan(that->lhs);
    scan(that->rhs);
}

void TreeScanner::visitConditional(JCConditional* that) {
    scan(that->cond);
    scan(that->truepart);
    scan(that->falsepart);
}

void TreeScanner::visitBinary(JCBinary* that) {
    scan(that->lhs);
    scan(that->rhs);
}

void TreeScanner::visitIndexed(JCArrayAccess* that) {
    scan(that->indexed);
    scan(that->index);
}

void TreeScanner::visitSelect(JCFieldAccess* that) {
    scan(that->selected);
}

void TreeScanner::visitIdent(JCIdent* that) {}

void TreeScanner::visitLiteral(JCLiteral* that) {}

void TreeScanner::visitTypeIdent(JCPrimitiveTypeTree* that) {}

void TreeScanner::visitTypeArray(JCArrayTypeTree* that) {
    scan(that->elementType);
}

void TreeScanner::visitModifiers(JCModifiers* that) {}

void TreeScanner::visitUnary(JCUnary* that) {
    scan(that->arg);
}

void TreeScanner::visitNewArray(JCNewArray* that) {
    scan(that->elementType);
    scan(that->dimens);
    scan(that->elems);
}
