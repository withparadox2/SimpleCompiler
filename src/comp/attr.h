//
// Created by withparadox2 on 2017/1/8.
//

#ifndef SIMPLECOMPILER_ATTR_H
#define SIMPLECOMPILER_ATTR_H

#include "../code/types.h"
#include "../tree/tree.h"
#include "env.h"
#include "../code/Flags.h"
#include "../tree/visitor.h"
#include "../code/type.h"

class Symtab;
class Enter;
class Names;

class Attr : public Visitor {
private:
    int pKind;
    Env* env;
    Symtab& syms;
    Names& names;
    //type expected in current visit pass
    TypePtr pt;
    TypePtr result;

    Attr();

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

    void visitUnary(JCUnary* that);

    void visitNewArray(JCNewArray* that);

public:
    static Attr& instance();

    void attrib(Env* env);

    void attribClass(ClassSymbol::Ptr c);

    TypePtr attribStat(Tree* tree, Env* env);

    template <typename T>
    TypePtr attribStats(std::vector<T>& list, Env* env);

    TypePtr attribExpr(Tree* tree, Env* env, TypePtr pt);

    TypePtr attribExpr(Tree* tree, Env* env);

    TypePtr attribType(Tree* tree, Env* env);

    TypePtr attribTree(Tree* tree, Env* env, int pkind, TypePtr pt);

    TypeList attribArgs(JCExpression::List &trees, Env* env);

    Symbol::Ptr resolveIdent(Env* env, const Name& name, int kind);

    Symbol::Ptr findType(Env* env, const Name& name);

    inline Enter& enter();
};

template <typename T>
TypePtr Attr::attribStats(std::vector<T>& list, Env* env) {
    for (auto iter = list.begin(); iter != list.end(); iter++) {
        attribStat(iter->get(), env);
    }
}

#endif //SIMPLECOMPILER_ATTR_H
