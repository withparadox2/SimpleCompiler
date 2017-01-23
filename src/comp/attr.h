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

    SymbolPtr resolveUnaryOperator(int optag, Env* env, TypePtr arg);

    SymbolPtr resolveBinaryOperator(int optag, Env* env, TypePtr left, TypePtr right);

    SymbolPtr resolveOperator(int optag, Env* env, Type::List argtypes);

    SymbolPtr findMethod(Env* env, TypePtr site, const Name& name, TypeList argTypes, bool isOperator);

    TypePtr newMethTemplate(TypeList argtypes);

    SymbolPtr resolveConstructor(Env* env, TypePtr site, TypeList argtypes);

    SymbolPtr selectSym(JCFieldAccess* tree, SymbolPtr sitesym, TypePtr site, Env* env, TypePtr pt, int pkind);

    TypePtr selectType(SymbolPtr sym);

public:
    static Attr& instance();

    void attrib(Env* env);

    void attribClass(ClassSymbol::Ptr c);

    TypePtr attribStat(Tree* tree, Env* env);

    template<typename T>
    TypePtr attribStats(std::vector<T>& list, Env* env);

    TypePtr attribExpr(Tree* tree, Env* env, TypePtr pt);

    TypePtr attribExpr(Tree* tree, Env* env);

    TypePtr attribType(Tree* tree, Env* env);

    TypePtr attribTree(Tree* tree, Env* env, int pkind, TypePtr pt);

    TypeList attribArgs(JCExpression::List& trees, Env* env);

    SymbolPtr resolveIdent(Env* env, const Name& name, int kind);

    SymbolPtr findType(Env* env, const Name& name);

    SymbolPtr findVar(Env* env, const Name& name);

    SymbolPtr findField(Env* env, const Name& name, TypePtr site, SymbolPtr c);

    inline Enter& enter();
};

template<typename T>
TypePtr Attr::attribStats(std::vector<T>& list, Env* env) {
    for (auto iter = list.begin(); iter != list.end(); iter++) {
        attribStat(iter->get(), env);
    }
}

#endif //SIMPLECOMPILER_ATTR_H
