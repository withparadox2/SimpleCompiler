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

class Symtab;
class Enter;

class Attr : public TreeScanner {
private:
    int pKind;
    Env* env;
    Symtab& syms;
    TypePtr result;

    Attr();

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

    void visitUnary(JCUnary* that);

    void visitNewArray(JCNewArray* that);

    void visitMethodInvocation(JCMethodInvocation* that);

public:
    static Attr& instance();

    void attrib(Env* env);

    void attribClass(ClassSymbol::Ptr c);

    TypePtr attribType(Tree* tree, Env* env);

    TypePtr attribTree(Tree* tree, Env* env, int pkind);

    Symbol::Ptr resolveIdent(Env* env, const Name& name, int kind);

    Symbol::Ptr findType(Env* env, const Name& name);

    inline Enter& enter();
};


#endif //SIMPLECOMPILER_ATTR_H
