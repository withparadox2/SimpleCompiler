//
// Created by withparadox2 on 2016/12/30.
//

#include "env.h"

Env::Env(Tree *tree, AttrContext *info)
        : tree(tree), info(info), next(nullptr), outer(nullptr), enclClass(nullptr), enclMethod(nullptr) {
}

AttrContext::AttrContext() : scope(nullptr), isSelfCall(false), selectSuper(false), varArgs(false) {
}
