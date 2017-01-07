//
// Created by withparadox2 on 2016/12/30.
//

#include "env.h"

Env::Env(Tree::Ptr tree, AttrContext* info)
        : tree(tree), info(info), next(nullptr), outer(nullptr), enclClass(nullptr), enclMethod(nullptr) {
}

Env* Env::dup(Tree::Ptr tree, AttrContext* info) {
    Env* env = new Env(*this);
    env->tree = tree;
    env->info.reset(info);
    return env;
}

Env::Env(const Env& env) : tree(env.tree), next(env.next), outer(env.outer), enclClass(env.enclClass),
                           enclMethod(env.enclMethod), info(nullptr) {
}

AttrContext::AttrContext() : scope(nullptr), isSelfCall(false), selectSuper(false), varArgs(false) {
}

AttrContext* AttrContext::dup(Scope::Ptr scope) {
    //will call default constructor
    AttrContext* ac = new AttrContext(*this);
    ac->scope = scope;
    return ac;
}

