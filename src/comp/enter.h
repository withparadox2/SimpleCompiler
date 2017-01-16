//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_ENTER_H
#define SIMPLECOMPILER_ENTER_H

#include <map>

#include "../tree/tree.h"
#include "../code/symbols.h"
#include "./env.h"

class Attr;
class Symtab;
class Names;
class ClassReader;

using std::map;

class Enter : public Visitor {
private:
    ClassReader& reader;
    Symtab& syms;
    Names& names;
    Attr& attr;

    Enter();

    Env* env;
public:

    //TODO delete env
    map<TypeSymbol::Ptr, Env*> typeEnvs;

    static Enter& instance();

    void complete(Tree* tree, Env* env);

    void visitClassDef(JCClassDecl* that) override;

    void visitMethodDef(JCMethodDecl* that) override;

    void visitVarDef(JCVariableDecl* that) override;

    void visitTree(Tree* that) override;

    Env* classEnv(JCClassDecl::Ptr& clazz);

    void completeMember(ClassSymbol::Ptr& c);

    Tree* defaultConstructor(ClassSymbol::Ptr& c);

    JCExpressionStatement* superCall(ClassSymbol::Ptr& c);

    Scope::Ptr& enterScope(Env* env);

    TypePtr signature(JCVariableDecl::List& params, JCExpression::Ptr& res, Env* env);

    Env* methodEnv(JCMethodDecl::Ptr& tree, Env* env);

};


#endif //SIMPLECOMPILER_ENTER_H
