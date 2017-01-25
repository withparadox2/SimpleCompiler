//
// Created by withparadox2 on 2016/12/30.
//

#ifndef SIMPLECOMPILER_ENV_H
#define SIMPLECOMPILER_ENV_H

#include "../code/scope.h"
#include "../code/symbol.h"
#include "../tree/tree.h"
#include <memory>

class AttrContext;

typedef std::shared_ptr<AttrContext> AttrPtr;

class Env {

public :
    typedef std::shared_ptr<Env> Ptr;
    /** The next enclosing environment.
     *
     *  If this env is generated from env1, then next point to env1.
     */
    Env::Ptr next;

    /** The environment enclosing the current class.
     *
     *  If current tree is a class, then *outer points to package env.
     */
    Env::Ptr outer;

    /** The tree with which this environment is associated.
     *
     *  Current tree. May be a package, class or method.
     */
    Tree::Ptr tree;

    /** The next enclosing class definition.
     *
     *  If current tree is a class, then it points to the tree.
     */
    JCClassDecl::Ptr enclClass;

    /** The next enclosing method definition.
     *
     *  If current tree is a method, then it points to the tree.
     */
    JCMethodDecl::Ptr enclMethod;

    /** A generic field for further information.
     *
     *  Must attatch to a Env obj.
     *
     *  May be shared by calling dup(tree), use shared_ptr
     */
    AttrPtr info;

    /** Is this an environment for evaluating a base clause?
     */
    bool baseClause;

    Env(Tree::Ptr tree, AttrPtr info);

    Env* dup(Tree::Ptr tree, AttrPtr info);

    Env* dup(Tree::Ptr tree);

    Env* dup(Tree* tree);

    Env* dup(Tree* tree, AttrPtr info);

    Env(const Env& env);
};

class AttrContext {
public:

    /** The scope of local symbols.
     */
    Scope::Ptr scope;
    /** Is this an environment for a this(...) or super(...) call?
     */
    bool isSelfCall;
    /** Are we evaluating the selector of a `super' or type name?
     */
    bool selectSuper;

    /** Are arguments to current function applications boxed into an array for varargs?
     */
    bool varArgs;

    AttrContext();

    AttrPtr dup(Scope::Ptr scope);
    AttrPtr dup();
};


#endif //SIMPLECOMPILER_ENV_H
