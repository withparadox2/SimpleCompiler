//
// Created by withparadox2 on 2017/1/8.
//

#include "attr.h"
Attr& Attr::instance() {
    static Attr attr;
    return attr;
}

Type::Ptr Attr::attribType(Tree* tree, Env* env) {
    return attribTree(tree, env, Kind::TYP);
}

Type::Ptr Attr::attribTree(Tree* tree, Env* env, Kind pkind) {
    Kind preKind = this->pKind;
    Env* preEnv = this->env;
    this->pKind = pkind;
    this->env = env;

    tree->accept(*this);

    this->pKind = preKind;
    this->env = preEnv;
    return Type::Ptr();
}

Attr::Attr() : pKind(Kind::ERR), env(nullptr){

}

void Attr::visitVarDef(JCVariableDecl& that) {

}

void Attr::visitIdent(JCIdent& that) {

}
