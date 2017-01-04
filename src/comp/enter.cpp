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
    ClassSymbol *c = reader.enterClass(that.name);
    c->memberField = new Scope(c);
    that.sym = c;
    Env *env = classEnv(&that);
    typeEnvs.insert(std::make_pair(c, env));
    //todo calc flags_field

    completeMember(c);
}

void Enter::visitMethodDef(JCMethodDecl &tree) {
    Scope &enclScope = enterScope(*this->env);
    MethodSymbol *m = new MethodSymbol(0, tree.name, nullptr, enclScope.owner);
    //TODO   m->flags
    tree.sym = m;

    //TODO calc type and parameters

    //TODO check unique
    enclScope.enter(m);

}

void Enter::visitTree(Tree &that) {
    //do nothing
}

Enter::Enter() : reader(ClassReader::instance()), syms(Symtab::instance()), names(Names::instance()), env(nullptr) {
}

Env *Enter::classEnv(JCClassDecl *clazz) {
    Env *local = new Env(clazz, new AttrContext);
    local->info->scope = new Scope(clazz->sym);
    local->enclClass = clazz;
    return local;
}

void Enter::completeMember(ClassSymbol *c) {
    ClassType *ct = static_cast<ClassType *>(c->type);

    Env *classEnv = typeEnvs.at(c);
    JCClassDecl *tree = dynamic_cast<JCClassDecl *>(classEnv->tree);

    c->flags |= Flags::UNATTRIBUTED;

    //Not support extend, super type must be Object type.
    ct->supertype_field = syms.objectType;

    if (!treeinfo::hasConstructors(tree->defs)) {
        //Not support anonymous class
        Tree *ctor = defaultConstructor(c);
        //TODO should prepend?
        tree->defs.push_back(Tree::Ptr(ctor));
    }

    if ((c->flags & Flags::INTERFACE) == 0) {
        Scope &scope = *classEnv->info->scope;
        long flag = Flags::FINAL | Flags::HASINIT;
        VarSymbol *thisSym =
                new VarSymbol(flag, *names._this, c->type, c);
        scope.enter(thisSym);

        //TODO exclude Object which shouldn't have super in scope.
        VarSymbol *superSym =
                new VarSymbol(flag, *names._super, ct->supertype_field, c);
        scope.enter(superSym);
    }

    this->env = classEnv;
    //enter members
    for (auto &item : tree->defs) {
        item->accept(*this);
    }
}

JCExpressionStatement *Enter::superCall(ClassSymbol *c) {
    //TODO figure out : x_0.super(x_1, ..., x_n)
    JCExpression::List args;
    return new JCExpressionStatement(new JCMethodInvocation(args, new JCIdent(*names._super)));
}

Tree *Enter::defaultConstructor(ClassSymbol *c) {
    //According to jls-8.8.9, default ctor shares a same access modifier with Class itself.
    JCModifiers *modifier = new JCModifiers((c->flags & Flags::AccessFlags) | Flags::GENERATEDCONSTR);

    JCStatement::List stats;
    stats.push_back(JCStatement::Ptr(superCall(c)));
    JCBlock *block = new JCBlock(stats);

    //no type, no arguments
    JCVariableDecl::List param;
    return new JCMethodDecl(modifier, nullptr, *names.init, param, block);
}

Scope &Enter::enterScope(const Env &env) {
    if (env.tree->treeTag == Tree::CLASSDEF) {
        return *dynamic_cast<JCClassDecl *>(env.tree)->sym->memberField;
    } else {
        return *env.info->scope;
    }
}

Type *Enter::signature(vector<JCVariableDecl *> *params, Tree *res, Env *env) {

    return nullptr;
}
