//
// Created by withparadox2 on 2017/1/24.
//

#ifndef SIMPLECOMPILER_GEN_H
#define SIMPLECOMPILER_GEN_H

#include <memory>
#include "../tree/alltree.h"
#include "../comp/env.h"
#include "../tree/visitor.h"
#include "Code.h"
#include "items.h"
#include "Pool.h"
#include "../code/type.h"

class Symtab;
class Names;

class GenContext {
public:
    typedef std::shared_ptr<GenContext> Ptr;

};

class Gen : public EnableMapHelper, public Visitor {
private:
    Gen();
    Env<GenContext>* env;
    /**Expected type.*/
    TypePtr pt;

    Code::Ptr code;
    Items::Ptr items;
    Pool::Ptr pool;

    Symtab& syms;
    Names& names;

    Item::Ptr result;

    MethodTypePtr methodType;

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

    void loadIntConst(int n);

    int makeRef(TypePtr type);

public:
    static Gen& instance();
    void genClass(Env<AttrContext>* env, JCClassDecl* cdef);

    void genDef(Tree* tree, Env<GenContext>* env);

    void genMethod(JCMethodDecl* tree, Env<GenContext>* env, bool fatcode);

    void initCode(JCMethodDecl* tree, Env<GenContext>* env);

    void genStat(Tree* tree, Env<GenContext>* env);

    template<typename T>
    void genStats(std::vector<T>& list, Env<GenContext>* env);

    void genArgs(JCExpression::List& trees, TypeList& pts);

    void genLoop(JCStatement* loop, JCStatement* body, JCExpression* cond, JCExpressionStatement::List step, bool testFirst);

    CondItem::Ptr genCond(Tree* tree);

    Item::Ptr genExpr(Tree* tree, TypePtr ptr);

    Item::Ptr completeBinop(Tree::Ptr lhs, Tree::Ptr rhs, OperatorSymbolPtr sym);

    Item::Ptr makeNewArray(TypePtr typePtr, int ndims);
};


#endif //SIMPLECOMPILER_GEN_H
