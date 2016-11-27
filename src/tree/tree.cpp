//
// Created by withparadox2 on 2016/11/14.
//

#include "tree.h"


JCModifiers::JCModifiers(int flags) : flags(flags) {
}

int JCModifiers::getTag() {
    return MODIFIERS;
}


JCPrimitiveTypeTree::JCPrimitiveTypeTree(int tag) : typetag(tag) {

}
int JCPrimitiveTypeTree::getTag() {
    return TYPEIDENT;
}
