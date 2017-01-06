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
#include "../code/Symtab.h"
#include <map>

using std::map;

class Enter : public Visitor {
private:
    ClassReader &reader;
    Symtab &syms;
    Names &names;

    Enter();

    Env *env;
public:
    map<TypeSymbol::Ptr, Env *> typeEnvs;

    static Enter &instance();

    void complete(Tree *tree);

    void visitClassDef(JCClassDecl &that) override;

    void visitMethodDef(JCMethodDecl &that) override;

    void visitTree(Tree &that) override;

    Env *classEnv(JCClassDecl *clazz);

    void completeMember(ClassSymbol::Ptr& c);

    Tree *defaultConstructor(ClassSymbol::Ptr& c);

    JCExpressionStatement *superCall(ClassSymbol::Ptr& c);

    Scope &enterScope(const Env &env);

    Type *signature(vector<JCVariableDecl *> *params, Tree *res, Env *env);

};


#endif //SIMPLECOMPILER_ENTER_H
