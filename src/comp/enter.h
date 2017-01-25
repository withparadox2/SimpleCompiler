//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_ENTER_H
#define SIMPLECOMPILER_ENTER_H

#include <map>
#include "../tree/tree.h"
#include "../code/symbols.h"
#include "./env.h"
#include "../tree/visitor.h"
#include "../util/context.h"

class Attr;
class Symtab;
class Names;
class ClassReader;

using std::map;

class Enter : public Visitor, public EnableMapHelper {
private:
    ClassReader* reader;
    Symtab* syms;
    Names* names;
    Attr* attr;

    Enter();

    Env<AttrContext>* env;
public:

    //Global env last to the end.
    map<TypeSymbolPtr, std::unique_ptr<Env<AttrContext>>> typeEnvs;

    static Enter& instance();

    void complete(Tree* tree, Env<AttrContext>* env);

    void visitClassDef(JCClassDecl* that) override;

    void visitMethodDef(JCMethodDecl* that) override;

    void visitVarDef(JCVariableDecl* that) override;

    void visitTree(Tree* that) override;

    Env<AttrContext>* classEnv(JCClassDecl::Ptr& clazz);

    void completeMember(ClassSymbolPtr& c);

    Tree* defaultConstructor(ClassSymbolPtr& c);

    JCExpressionStatement* superCall(ClassSymbolPtr& c);

    Scope::Ptr& enterScope(Env<AttrContext>* env);

    TypePtr signature(JCVariableDecl::List& params, JCExpression::Ptr& res, Env<AttrContext>* env);

    Env<AttrContext>* methodEnv(JCMethodDecl::Ptr tree, Env<AttrContext>* env);
};


#endif //SIMPLECOMPILER_ENTER_H
