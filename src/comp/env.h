//
// Created by withparadox2 on 2016/12/30.
//

#ifndef SIMPLECOMPILER_ENV_H
#define SIMPLECOMPILER_ENV_H

#include "../code/scope.h"
#include "../code/symbol.h"
#include "../tree/tree.h"

class AttrContext;

class Env {

public :
    /** The next enclosing environment.
     *
     * TODO figure out what's this
     */
    Env *next;

    /** The environment enclosing the current class.
     *
     *  If current tree is a class, then *outer points to package env.
     */
    Env *outer;

    /** The tree with which this environment is associated.
     *
     *  Current tree. May be a package, class or method.
     */
    Tree *tree;

    /** The next enclosing class definition.
     *
     *  If current tree is a class, then it points to the tree.
     */
    JCClassDecl *enclClass;

    /** The next enclosing method definition.
     *
     *  If current tree is a method, then it points to the tree.
     */
    JCMethodDecl *enclMethod;

    /** A generic field for further information.
     */
    AttrContext *info;

    /** Is this an environment for evaluating a base clause?
     */
    bool baseClause;

    Env(Tree *tree, AttrContext *info);
};

class AttrContext {
    /** The scope of local symbols.
     */
    Scope *scope;
    /** Is this an environment for a this(...) or super(...) call?
     */
    bool isSelfCall;
    /** Are we evaluating the selector of a `super' or type name?
     */
    bool selectSuper;

    /** Are arguments to current function applications boxed into an array for varargs?
     */
    bool varArgs;
public:
    AttrContext();
};


#endif //SIMPLECOMPILER_ENV_H
