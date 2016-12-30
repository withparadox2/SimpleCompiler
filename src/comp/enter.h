//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_ENTER_H
#define SIMPLECOMPILER_ENTER_H

#include "../tree/tree.h"
#include "../code/symbol.h"
#include "../util/names.h"
#include "../jvm/ClassReader.h"
#include "./env.h"
#include <map>

using std::map;

class Enter : public Visitor {
private:
    ClassReader &reader;
    Enter();
public:
    map<TypeSymbol *, Env *> typeEnvs;
    static Enter &instance();
    void complete(Tree *tree);

    void visitClassDef(JCClassDecl &that) override;

    void visitMethodDef(JCMethodDecl &that) override;

    void visitTree(Tree &that) override;

    Env *classEnv(JCClassDecl *clazz);

};


#endif //SIMPLECOMPILER_ENTER_H
