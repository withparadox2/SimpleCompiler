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
#include "../util/context.h"

class Symtab;

class Enter;

class Names;

class Attr : public Visitor, public EnableMapHelper {
private:
    int pKind;
    Env<AttrContext>* env;
    Symtab* syms;
    Names* names;
    Enter* enter;
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

    SymbolPtr resolveUnaryOperator(int optag, Env<AttrContext>* env, TypePtr arg);

    SymbolPtr resolveBinaryOperator(int optag, Env<AttrContext>* env, TypePtr left, TypePtr right);

    SymbolPtr resolveOperator(int optag, Env<AttrContext>* env, TypeList argtypes);

    TypePtr newMethTemplate(TypeList argtypes);

    SymbolPtr resolveConstructor(Env<AttrContext>* env, TypePtr site, TypeList argtypes);

    SymbolPtr selectSym(JCFieldAccess* tree, SymbolPtr sitesym, TypePtr site, Env<AttrContext>* env, TypePtr pt, int pkind);

    TypePtr selectType(SymbolPtr sym);

    bool checkTypes(TypeList src, TypeList dest);

public:
    static Attr& instance();

    void attrib(Env<AttrContext>* env);

    void attribClass(ClassSymbolPtr c);

    TypePtr attribStat(Tree* tree, Env<AttrContext>* env);

    template<typename T>
    void attribStats(std::vector<T>& list, Env<AttrContext>* env);

    TypePtr attribExpr(Tree* tree, Env<AttrContext>* env, TypePtr pt);

    TypePtr attribExpr(Tree* tree, Env<AttrContext>* env);

    TypeList attribExprs(JCExpression::List trees, Env<AttrContext>* env, TypePtr pt);

    TypePtr attribType(Tree* tree, Env<AttrContext>* env);

    TypePtr attribTree(Tree* tree, Env<AttrContext>* env, int pkind, TypePtr pt);

    TypeList attribArgs(JCExpression::List& trees, Env<AttrContext>* env);

    SymbolPtr resolveIdent(Env<AttrContext>* env, const Name& name, int kind);

    SymbolPtr findType(Env<AttrContext>* env, const Name& name);

    SymbolPtr findVar(Env<AttrContext>* env, const Name& name);

    SymbolPtr findField(Env<AttrContext>* env, const Name& name, TypePtr site, SymbolPtr c);

    SymbolPtr findMethod(Env<AttrContext>* env, TypePtr site, const Name& name, TypeList argTypes, bool isOperator);
};

template<typename T>
void Attr::attribStats(std::vector<T>& list, Env<AttrContext>* env) {
    for (auto iter = list.begin(); iter != list.end(); iter++) {
        attribStat(iter->get(), env);
    }
}

#endif //SIMPLECOMPILER_ATTR_H
