//
// Created by withparadox2 on 2016/11/14.
//

#include "tree.h"


JCModifiers::JCModifiers(int flags) : Tree(MODIFIERS), flags(flags) {
}


JCPrimitiveTypeTree::JCPrimitiveTypeTree(int tag) : Tree(TYPEIDENT), typetag(tag) {
}

JCArrayTypeTree::JCArrayTypeTree(JCExpression *elementType) : Tree(TYPEARRAY), elementType(elementType) {

}

Tree::Tree(int tag) : treeTag(tag) {
}

JCIdent::JCIdent(Name &name) : treeTag(TYPEIDENT), name(name) {

}

JCFieldAccess::JCFieldAccess(JCExpression *selected, Name &selector) : Tree(SELECT), selected(selected),
                                                                       selector(selector) {

}
