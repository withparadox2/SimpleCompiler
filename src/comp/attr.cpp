//
// Created by withparadox2 on 2017/1/8.
//

#include <iostream>
#include "attr.h"
Attr& Attr::instance() {
    static Attr attr;
    return attr;
}

Type::Ptr Attr::attribType(Tree* tree, Env* env) {
    return attribTree(tree, env, Kind::TYP);
}

Type::Ptr Attr::attribTree(Tree* tree, Env* env, int pkind) {
    int preKind = this->pKind;
    Env* preEnv = this->env;
    this->pKind = pkind;
    this->env = env;

    tree->accept(this);

    this->pKind = preKind;
    this->env = preEnv;
    return Type::Ptr();
}

Attr::Attr() : pKind(Kind::ERR), env(nullptr), syms(Symtab::instance()){

}

void Attr::visitVarDef(JCVariableDecl* that) {

}

void Attr::visitIdent(JCIdent* that) {
    Symbol::Ptr sym;
    //TODO why it is necessary to check kind()
    if (that->sym && that->sym->kind != Kind::VAR) {
        sym = that->sym;
    } else {
        sym = Symbol::Ptr(resolveIdent(env, that->name, pKind));
    }
}

void Attr::visitTypeArray(JCArrayTypeTree* that) {
    TypePtr eType = attribType(that->elementType.get(), this->env);
    result = TypePtr(new ArrayType(eType, syms.arrayClass));
}

Symbol::Ptr Attr::resolveIdent(Env* env, const Name& name, int kind) {
    if ((kind & Kind::TYP) != 0) {
        return findType(env, name);
    }
    return syms.noSymbol;
}

Symbol::Ptr Attr::findType(Env* env, const Name& name) {
    for(Env* env1 = env; env1->outer; env1 = env1->outer.get()) {
        Symbol::Ptr sym = env1->info->scope->lookUp(name);
        if (sym && sym->kind == Kind::TYP) {
            return sym;
        }
    }

    StarImportScope& gScope = StarImportScope::instance();

    Symbol::Ptr sym = gScope.lookUp(name);
    if (sym) {
        std::cout << sym->name.desc << std::endl;
    }

    return syms.noSymbol;
}
