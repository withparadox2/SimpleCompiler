//
// Created by withparadox2 on 2017/1/8.
//

#ifndef SIMPLECOMPILER_ATTR_H
#define SIMPLECOMPILER_ATTR_H

#include "../code/type.h"
#include "../tree/tree.h"
#include "env.h"
#include "../code/Symtab.h"
#include "../code/Flags.h"

class Attr : public Visitor {
private:
    int pKind;
    Env* env;
    Symtab& syms;
    Type::Ptr result;

    Attr();

public:
    static Attr& instance();

    Type::Ptr attribType(Tree* tree, Env* env);

    Type::Ptr attribTree(Tree* tree, Env* env, int pkind);

    void visitVarDef(JCVariableDecl& that);

    void visitIdent(JCIdent& that);

    void visitTypeArray(JCArrayTypeTree& that);

    Symbol::Ptr resolveIdent(Env* env, const Name& name, int kind);

    Symbol::Ptr findType(Env* env, const Name& name);
};


#endif //SIMPLECOMPILER_ATTR_H
