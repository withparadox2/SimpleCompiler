//
// Created by withparadox2 on 2016/12/19.
//

#include "enter.h"
#include "../util/error.h"
#include "attr.h"
#include "../jvm/ClassReader.h"
#include "../code/Symtab.h"

#define KEY_ENTER "enter"

Enter& Enter::instance() {
    Enter* inst = Context::instance().get<Enter>(KEY_ENTER);
    if (inst == nullptr) {
        inst = new Enter();
    }
    return *inst;
}

void Enter::complete(Tree* tree, Env* env) {
    Env* preEnv = this->env;
    this->env = env;
    tree->accept(this);
    this->env = preEnv;
}

void Enter::visitClassDef(JCClassDecl* that) {
    ClassSymbolPtr& c = reader->enterClass(that->name);
    c->memberField = Scope::Ptr(new Scope(c));
    that->sym = c;
    JCClassDecl::Ptr classSharedPtr = std::dynamic_pointer_cast<JCClassDecl>(that->shared_from_this());
    Env* env = classEnv(classSharedPtr);
    typeEnvs.insert(std::make_pair(c, std::unique_ptr<Env>(env)));
    //TODO calc flags_field

    //TODO Technically, we should use env wrapped this class symbol.
    env->info->scope->enter(c);
    completeMember(c);
}

void Enter::visitMethodDef(JCMethodDecl* tree) {
    Scope::Ptr& enclScope = enterScope(this->env);
    MethodSymbolPtr m(new MethodSymbol(0, tree->name, nullptr, enclScope->owner.lock()));
    //TODO   m->flags
    tree->sym = m;
    m->type = signature(tree->params, tree->resType, this->env);

    //TODO check unique
    enclScope->enter(m);
}

void Enter::visitVarDef(JCVariableDecl* that) {
    //ignore static or interface
    attr->attribType(that->vartype.get(), env);
    Scope::Ptr enclScope = enterScope(env);
    VarSymbolPtr v(new VarSymbol(0, that->name, that->vartype->type, enclScope->owner.lock()));
    that->sym = v;
    //TODO init final var

    enclScope->enter(v);
}

void Enter::visitTree(Tree* that) {
    //do nothing
}

Enter::Enter() : env(nullptr) {
    Context::instance().put(KEY_ENTER, this);
    reader = &ClassReader::instance();
    syms = &Symtab::instance();
    names = &Names::instance();
    attr = &Attr::instance();
}

Env* Enter::classEnv(JCClassDecl::Ptr& clazz) {
    Env* local = new Env(clazz, AttrPtr(new AttrContext));
    local->info->scope = Scope::Ptr(new Scope(clazz->sym));
    local->enclClass = clazz;
    return local;
}

void Enter::completeMember(ClassSymbolPtr& c) {
    ClassType* ct = static_cast<ClassType*>(c->type.get());

    Env* classEnv = typeEnvs.at(c).get();
    JCClassDecl* tree = dynamic_cast<JCClassDecl*>(classEnv->tree.get());

    c->flags |= Flags::UNATTRIBUTED;

    //Not support extend, super type must be Object type.
    ct->supertype_field = syms->objectType;

    if (!treeinfo::hasConstructors(tree->defs)) {
        //Not support anonymous class
        Tree* ctor = defaultConstructor(c);
        tree->defs.push_back(Tree::Ptr(ctor));
    }

    if ((c->flags & Flags::INTERFACE) == 0) {
        Scope& scope = *classEnv->info->scope;
        long flag = Flags::FINAL | Flags::HASINIT;
        VarSymbolPtr thisSym(new VarSymbol(flag, *names->_this, c->type, c));
        scope.enter(thisSym);

        //TODO exclude Object which shouldn't have super in scope.
        VarSymbolPtr superSym(new VarSymbol(flag, *names->_super, ct->supertype_field, c));
        scope.enter(superSym);
    }

    this->env = classEnv;
    //enter members
    for (auto& item : tree->defs) {
        item->accept(this);
    }
}

JCExpressionStatement* Enter::superCall(ClassSymbolPtr& c) {
    //TODO figure out : x_0.super(x_1, ..., x_n)
    JCExpression::List args;
    return new JCExpressionStatement(new JCMethodInvocation(args, new JCIdent(*names->_super)));
}

Tree* Enter::defaultConstructor(ClassSymbolPtr& c) {
    //According to jls-8.8.9, default ctor shares a same access modifier with Class itself.
    JCModifiers* modifier = new JCModifiers((c->flags & Flags::AccessFlags) | Flags::GENERATEDCONSTR);

    JCStatement::List stats;
    stats.push_back(JCStatement::Ptr(superCall(c)));
    JCBlock* block = new JCBlock(stats);

    //no type, no arguments
    JCVariableDecl::List param;
    return new JCMethodDecl(modifier, nullptr, *names->init, param, block);
}

Scope::Ptr& Enter::enterScope(Env* env) {
    if (env->tree->treeTag == Tree::CLASSDEF) {
        return dynamic_cast<JCClassDecl*>(env->tree.get())->sym->memberField;
    } else {
        return env->info->scope;
    }
}

TypePtr Enter::signature(JCVariableDecl::List& params, JCExpression::Ptr& res, Env* env) {
    TypeList args;
    for (auto iter = params.begin(); iter != params.end(); iter++) {
        complete(iter->get(), env);
        args.push_back(iter->get()->vartype->type);
    }
    TypePtr restype = !res ? syms->voidType : attr->attribType(res.get(), env);

    return MethodTypePtr(new MethodType(args, restype, syms->methodClass));
}

Env* Enter::methodEnv(JCMethodDecl::Ptr tree, Env* env) {
    Scope::Ptr ptr = env->info->scope->dupUnshared();
    Env* localEnv = env->dup(tree, env->info->dup(ptr));
    localEnv->enclMethod = tree;
    localEnv->info->scope->owner = tree->sym;
    //ignore static level
    return localEnv;
}
