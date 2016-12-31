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

Enter::Enter() : reader(ClassReader::instance()), syms(Symtab::instance()), names(Names::instance()) {
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
        //Not support anonymous class
        Tree *ctor = defaultConstructor(symbol);
        //TODO should prepend?
        tree->defs->push_back(ctor);
    }

    //TODO enter this/super

    //enter members
    for(auto &item : *(tree->defs)) {
        item->accept(*this);
    }
}

JCExpressionStatement *Enter::superCall(ClassSymbol *c) {
    //TODO figure out : x_0.super(x_1, ..., x_n)
    return new JCExpressionStatement(new JCMethodInvocation(nullptr, new JCIdent(*names._super)));
}

Tree *Enter::defaultConstructor(ClassSymbol *c) {
    //According to jls-8.8.9, default ctor shares a same access modifier with Class itself.
    JCModifiers *modifier = new JCModifiers((c->flags & Flags::AccessFlags) | Flags::GENERATEDCONSTR);

    vector<JCStatement *> *stats = new vector<JCStatement *>();
    stats->push_back(superCall(c));
    JCBlock *block = new JCBlock(stats);

    //no type, no arguments
    return new JCMethodDecl(modifier, nullptr, names.init, nullptr, block);
}
