//
// Created by withparadox2 on 2016/12/19.
//

#include "enter.h"
#include "../util/error.h"
#include "../code/Flags.h"

Enter& Enter::instance() {
    static Enter inst;
    return inst;
}

void Enter::complete(Tree* tree, Env* env) {
    Env* preEnv = this->env;
    this->env = env;
    tree->accept(this);
    this->env = preEnv;
}

void Enter::visitClassDef(JCClassDecl* that) {
    ClassSymbol::Ptr& c = reader.enterClass(that->name);
    c->memberField = Scope::Ptr(new Scope(c));
    that->sym = c;
    JCClassDecl::Ptr classSharedPtr = std::dynamic_pointer_cast<JCClassDecl>(that->shared_from_this());
    Env* env = classEnv(classSharedPtr);
    typeEnvs.insert(std::make_pair(c, env));
    //TODO calc flags_field

    completeMember(c);
}

void Enter::visitMethodDef(JCMethodDecl* tree) {
    Scope::Ptr& enclScope = enterScope(this->env);
    MethodSymbol::Ptr m(new MethodSymbol(0, tree->name, nullptr, enclScope->owner.lock()));
    //TODO   m->flags
    tree->sym = m;
    m->type = signature(tree->params, tree->resType, this->env);
    //TODO calc type and parameters

    //TODO check unique
    enclScope->enter(m);
}

void Enter::visitVarDef(JCVariableDecl* that) {
    //ignore static or interface
    attr.attribType(that->vartype.get(), env);
}

void Enter::visitTree(Tree* that) {
    //do nothing
}

Enter::Enter() : reader(ClassReader::instance()),
                 syms(Symtab::instance()),
                 names(Names::instance()),
                 env(nullptr),
                 attr(Attr::instance()) {
}

Env* Enter::classEnv(JCClassDecl::Ptr& clazz) {
    Env* local = new Env(clazz, new AttrContext);
    local->info->scope = Scope::Ptr(new Scope(clazz->sym));
    local->enclClass = clazz;
    return local;
}

void Enter::completeMember(ClassSymbol::Ptr& c) {
    ClassType* ct = static_cast<ClassType*>(c->type.get());

    Env* classEnv = typeEnvs.at(c);
    JCClassDecl* tree = dynamic_cast<JCClassDecl*>(classEnv->tree.get());

    c->flags |= Flags::UNATTRIBUTED;

    //Not support extend, super type must be Object type.
    ct->supertype_field = syms.objectType;

    if (!treeinfo::hasConstructors(tree->defs)) {
        //Not support anonymous class
        Tree* ctor = defaultConstructor(c);
        tree->defs.push_back(Tree::Ptr(ctor));
    }

    if ((c->flags & Flags::INTERFACE) == 0) {
        Scope& scope = *classEnv->info->scope;
        long flag = Flags::FINAL | Flags::HASINIT;
        VarSymbol::Ptr thisSym(new VarSymbol(flag, *names._this, c->type, c));
        scope.enter(thisSym);

        //TODO exclude Object which shouldn't have super in scope.
        VarSymbol::Ptr superSym(new VarSymbol(flag, *names._super, ct->supertype_field, c));
        scope.enter(superSym);
    }

    this->env = classEnv;
    //enter members
    for (auto& item : tree->defs) {
        item->accept(this);
    }
}

JCExpressionStatement* Enter::superCall(ClassSymbol::Ptr& c) {
    //TODO figure out : x_0.super(x_1, ..., x_n)
    JCExpression::List args;
    return new JCExpressionStatement(new JCMethodInvocation(args, new JCIdent(*names._super)));
}

Tree* Enter::defaultConstructor(ClassSymbol::Ptr& c) {
    //According to jls-8.8.9, default ctor shares a same access modifier with Class itself.
    JCModifiers* modifier = new JCModifiers((c->flags & Flags::AccessFlags) | Flags::GENERATEDCONSTR);

    JCStatement::List stats;
    stats.push_back(JCStatement::Ptr(superCall(c)));
    JCBlock* block = new JCBlock(stats);

    //no type, no arguments
    JCVariableDecl::List param;
    return new JCMethodDecl(modifier, nullptr, *names.init, param, block);
}

Scope::Ptr& Enter::enterScope(Env* env) {
    if (env->tree->treeTag == Tree::CLASSDEF) {
        return dynamic_cast<JCClassDecl*>(env->tree.get())->sym->memberField;
    } else {
        return env->info->scope;
    }
}

Type::Ptr Enter::signature(JCVariableDecl::List& params, JCExpression::Ptr& res, Env* env) {

    for (auto iter = params.begin(); iter != params.end(); iter++) {
        complete(iter->get(), env);
    }
    return nullptr;
}

Env *Enter::methodEnv(JCMethodDecl::Ptr &tree, Env *env) {
    return nullptr;
}
