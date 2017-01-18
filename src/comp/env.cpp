//
// Created by withparadox2 on 2016/12/30.
//

#include "env.h"

Env::Env(Tree::Ptr tree, AttrPtr info)
        : tree(tree), info(info), next(nullptr), outer(nullptr), enclClass(nullptr), enclMethod(nullptr) {
}

Env* Env::dup(Tree::Ptr tree) {
    return dup(tree, info);
}

Env* Env::dup(Tree::Ptr tree, AttrPtr info) {
    Env* env = new Env(*this);
    env->tree = tree;
    env->info = info;
    return env;
}

Env::Env(const Env& env) : tree(env.tree), next(env.next), outer(env.outer), enclClass(env.enclClass),
                           enclMethod(env.enclMethod), info(nullptr) {
}

Env* Env::dup(Tree* tree) {
    return dup(tree->shared_from_this());
}

Env* Env::dup(Tree* tree, AttrPtr info) {
    return dup(tree->shared_from_this(), info);
}

AttrContext::AttrContext() : scope(nullptr), isSelfCall(false), selectSuper(false), varArgs(false) {
}

AttrPtr AttrContext::dup(Scope::Ptr scope) {
    //will call default constructor
    AttrPtr ac(new AttrContext(*this));
    ac->scope = scope;
    return ac;
}

AttrPtr AttrContext::dup() {
    return dup(scope);
}

