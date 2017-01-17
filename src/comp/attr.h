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

class Attr : public Visitor {
private:
    int pKind;
    Env* env;
    Symtab& syms;
    TypePtr result;

    Attr();

public:
    static Attr& instance();

    void attrib(Env* env);

    void attribClass(ClassSymbol::Ptr c);

    TypePtr attribType(Tree* tree, Env* env);

    TypePtr attribTree(Tree* tree, Env* env, int pkind);

    void visitVarDef(JCVariableDecl* that);

    void visitIdent(JCIdent* that);

    void visitTypeArray(JCArrayTypeTree* that);

    void visitTypeIdent(JCPrimitiveTypeTree* that);

    void visitMethodDef(JCMethodDecl* that);

    Symbol::Ptr resolveIdent(Env* env, const Name& name, int kind);

    Symbol::Ptr findType(Env* env, const Name& name);

    inline Enter& enter();
};


#endif //SIMPLECOMPILER_ATTR_H
