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

JCIdent::JCIdent(Name &name) : Tree(TYPEIDENT), name(name) {

}

JCFieldAccess::JCFieldAccess(JCExpression *selected, Name &selector) : Tree(SELECT), selected(selected),
                                                                       selector(selector) {

}

JCVariableDecl::JCVariableDecl(JCExpression *type, Name &name) : Tree(VARDEF), type(type), name(name) {

}

JCBlock::JCBlock(vector<JCStatement *> *stats) : Tree(BLOCK), stats(stats) {

}

JCIf::JCIf(JCExpression *cond, JCStatement *thenpart, JCStatement *elsepart) : Tree(IF), cond(cond), thenPart(thenpart),
                                                                               elsePart(elsepart) {

}

JCExpressionStatement::JCExpressionStatement(JCExpression *exp) : Tree(EXEC), exp(exp) {

}

JCForLoop::JCForLoop(vector<JCStatement *> *init, JCExpression *cond, vector<JCExpressionStatement *> *step,
                     JCStatement *body) : Tree(FORLOOP), init(init), cond(cond), step(step), body(body) {

}

JCMethodDecl::JCMethodDecl(JCModifiers *mods, JCExpression *type, Name *name, vector<JCExpression *> *params,
                           JCBlock *body) : Tree(METHODDEF), mods(mods), type(type), name(name), params(params), body(body){

}
