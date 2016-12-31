//
// Created by withparadox2 on 2016/12/19.
//

#include "enter.h"
#include "../util/error.h"
#include "../code/Flags.h"

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

    completeMember(symbol);
}

void Enter::visitMethodDef(JCMethodDecl &that) {

}

void Enter::visitTree(Tree &that) {
    //do nothing
}

Enter::Enter() : reader(ClassReader::instance()), syms(Symtab::instance()) {
}

Env *Enter::classEnv(JCClassDecl *clazz) {
    Env *local = new Env(clazz, new AttrContext);
    local->enclClass = clazz;
    return local;
}

void Enter::completeMember(ClassSymbol *symbol) {
    Env *classEnv = typeEnvs.at(symbol);
    JCClassDecl *tree = dynamic_cast<JCClassDecl *>(classEnv->tree);

    symbol->flags |= Flags::UNATTRIBUTED;

    //Not support extend, super type must be Object type.
    Type *superType = syms.objectType;

    if (!treeinfo::hasConstructors(*tree->defs)) {

    }
    //enter members
    for(auto &item : *(tree->defs)) {
        item->accept(*this);
    }
}
