//
// Created by withparadox2 on 2016/12/19.
//

#include "enter.h"
#include "../util/error.h"

Enter &Enter::instance() {
    static Enter inst;
    return inst;
}

void Enter::complete(Tree *tree) {
    tree->accept(*this);
}

void Enter::visitClassDef(JCClassDecl &that) {
    ClassSymbol *symbol = reader.enterClass(that.name);
    that.sym = symbol;
    Env *env = classEnv(&that);
    typeEnvs.insert(std::make_pair(symbol, env));
    //todo calc flags_field

    //enter members
    for(auto &item : *that.defs) {
        item->accept(*this);
    }
}

void Enter::visitMethodDef(JCMethodDecl &that) {

}

void Enter::visitTree(Tree &that) {
    //do nothing
}

Enter::Enter() : reader(ClassReader::instance()) {
}

Env *Enter::classEnv(JCClassDecl *clazz) {
    Env *local = new Env(clazz, new AttrContext);
    local->enclClass = clazz;
    return local;
}
