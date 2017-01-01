//
// Created by withparadox2 on 2016/12/30.
//

#include "env.h"

Env::Env(Tree *tree, AttrContext *info)
        : tree(tree), info(info), next(nullptr), outer(nullptr), enclClass(nullptr), enclMethod(nullptr) {
}

Env *Env::dup(Tree *tree, AttrContext *info) {
    Env *env = new Env(*this);
    env->tree = tree;
    env->info = info;
    return env;
}

AttrContext::AttrContext() : scope(nullptr), isSelfCall(false), selectSuper(false), varArgs(false) {
}

AttrContext *AttrContext::dup(Scope *scope) {
    //will call default constructor
    AttrContext *ac = new AttrContext(*this);
    ac->scope = scope;
    return ac;
}
