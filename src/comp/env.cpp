//
// Created by withparadox2 on 2016/12/30.
//

#include "env.h"



AttrContext::AttrContext() : scope(nullptr), isSelfCall(false), selectSuper(false), varArgs(false) {
}

AttrContext::Ptr AttrContext::dup(Scope::Ptr scope) {
    //will call default constructor
    Ptr ac(new AttrContext(*this));
    ac->scope = scope;
    return ac;
}

AttrContext::Ptr AttrContext::dup() {
    return dup(scope);
}

