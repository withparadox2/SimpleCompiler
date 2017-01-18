//
// Created by withparadox2 on 2017/1/8.
//

#include <iostream>
#include "attr.h"
#include "../util/log.h"
#include "../code/Symtab.h"
#include "enter.h"
#include "../util/error.h"
#include "../util/names.h"

Attr& Attr::instance() {
    static Attr attr;
    return attr;
}

Type::Ptr Attr::attribType(Tree* tree, Env* env) {
    return attribTree(tree, env, Kind::TYP, syms.noType);
}

TypePtr Attr::attribExpr(Tree* tree, Env* env, TypePtr pt) {
    return attribTree(tree, env, Kind::VAL,
                      pt->tag != TypeTags::ERROR ? pt : syms.noType);
}


TypePtr Attr::attribExpr(Tree* tree, Env* env) {
    return attribExpr(tree, env, syms.noType);
}


TypePtr Attr::attribStat(Tree* tree, Env* env) {
    return attribTree(tree, env, Kind::NIL, syms.noType);
}


Type::Ptr Attr::attribTree(Tree* tree, Env* env, int pkind, TypePtr pt) {
    TypePtr prePt = this->pt;
    int preKind = this->pKind;
    Env* preEnv = this->env;
    this->pt = pt;
    this->pKind = pkind;
    this->env = env;

    tree->accept(this);

    this->pKind = preKind;
    this->env = preEnv;
    this->pt = prePt;
    return result;
}

Attr::Attr() : pKind(Kind::ERR), env(nullptr), syms(Symtab::instance()), names(Names::instance()) {

}

void Attr::visitVarDef(JCVariableDecl* that) {
    if (!that->sym) {
        that->sym = VarSymbol::Ptr(new VarSymbol(0, that->name, that->type, syms.noSymbol));
    }
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
    that->type = result;
}

Symbol::Ptr Attr::resolveIdent(Env* env, const Name& name, int kind) {
    if ((kind & Kind::TYP) != 0) {
        return findType(env, name);
    }
    return syms.noSymbol;
}

Symbol::Ptr Attr::findType(Env* env, const Name& name) {
    for (Env* env1 = env; env1->outer; env1 = env1->outer.get()) {
        Symbol::Ptr sym = env1->info->scope->lookUp(name);
        if (sym && sym->kind == Kind::TYP) {
            return sym;
        }
    }

    StarImportScope& gScope = StarImportScope::instance();

    Symbol::Ptr sym = gScope.lookUp(name);
    if (sym) {
        log("find type : " + sym->name.desc);
    }
    return syms.noSymbol;
}

void Attr::visitTypeIdent(JCPrimitiveTypeTree* that) {
    result = syms.typeOfTag[that->typetag];
    that->type = result;
}

void Attr::attrib(Env* env) {
    attribClass(env->enclClass->sym);
}

void Attr::attribClass(ClassSymbol::Ptr c) {
    Env* env = enter().typeEnvs.at(c);
    JCClassDecl::Ptr tree = env->enclClass;
    for (auto iter = tree->defs.begin(); iter != tree->defs.end(); iter++) {
        (*iter)->accept(this);
    }
}

void Attr::visitMethodDef(JCMethodDecl* that) {
    log("attr method : " + that->name.desc);
    //ignore any check.
    MethodSymbol::Ptr m = that->sym;
    JCMethodDecl::Ptr treePtr = std::dynamic_pointer_cast<JCMethodDecl>
            (that->shared_from_this());
    Env::Ptr localEnv(enter().methodEnv(treePtr, env));

    // Attribute all value parameters.
    for (auto iter = that->params.begin(); iter != that->params.end(); iter++) {
        attribStat(iter->get(), localEnv.get());
    }

    attribStat(that->body.get(), localEnv.get());
}

Enter& Attr::enter() {
    return Enter::instance();
}

void Attr::visitBlock(JCBlock* that) {
    //Not support static block or instance initialize
    Env::Ptr localEnv(env->dup(that, env->info->dup(env->info->scope->dup())));
    attribStats(that->stats, localEnv.get());
    localEnv->info->scope->leave();
}

void Attr::visitForLoop(JCForLoop* that) {
    Env::Ptr loopEnv(env->dup(env->tree, env->info->dup(env->info->scope->dup())));
    attribStats(that->init, loopEnv.get());
    if (that->cond) {
        attribExpr(that->cond.get(), loopEnv.get(), syms.booleanType);
    }
    // before, we were not in loop!
    attribStats(that->step, loopEnv.get());
    attribStat(that->body.get(), loopEnv.get());
    loopEnv->info->scope->leave();
    result = nullptr;
}

void Attr::visitIf(JCIf* that) {
    attribExpr(that->cond.get(), env, syms.booleanType);
    attribStat(that->thenPart.get(), env);
    if (that->elsePart) {
        attribStat(that->elsePart.get(), env);
    }
    //TODO checkEmpty
    result = nullptr;
}

void Attr::visitExec(JCExpressionStatement* that) {
    Env::Ptr localEnv(env->dup(that));
    attribExpr(that->exp.get(), localEnv.get());
    result = nullptr;
}

void Attr::visitBreak(JCBreak* that) {
    that->target = env->tree;
    result = nullptr;
}

void Attr::visitContinue(JCContinue* that) {
    that->target = env->tree;
    result = nullptr;
}

void Attr::visitReturn(JCReturn* that) {
    if (!env->enclMethod ||
            env->enclMethod->sym->owner != env->enclClass->sym) {
        error("return outside method");
    } else {
        SymbolPtr m = env->enclMethod->sym;
        if (m->type->getReturnType()->tag == TypeTags::VOID) {
            if (that->expr) {
                error("can not return val from method with return type of void");
            }
        } else if (!that->expr) {
            error("missing expr of return statement");
        } else {
            attribExpr(that->expr.get(), env, m->type->getReturnType());
        }
    }
}

//TODO
void Attr::visitApply(JCMethodInvocation* that) {
    Env::Ptr localEnv(env->dup(that, env->info->dup()));
    Name* methName = treeinfo::name(that->meth.get());
    bool isConstructor =
            methName == names._this || methName == names._super;
    if (isConstructor) {
        //We are seeing ...this(...) or ...super(...)
        //TODO checkFirstConstructorStat
        localEnv->info->isSelfCall = true;
        TypeList argtypes = attribArgs(that->args, localEnv.get());
    } else {

    }

}

void Attr::visitNewClass(JCNewClass* that) {
}

void Attr::visitParens(JCParens* that) {
    //TODO why env, pKind, pt
    that->type = attribTree(that->expr.get(), env, pKind, pt);
    result = that->type;
}

void Attr::visitAssign(JCAssign* that) {

}

void Attr::visitConditional(JCConditional* that) {
    attribExpr(that->cond.get(), env, syms.booleanType);
    attribExpr(that->truepart.get(), env);
    attribExpr(that->falsepart.get(), env);
    //assume types is correct
    result = that->truepart->type;
}

void Attr::visitBinary(JCBinary* that) {

}

void Attr::visitIndexed(JCArrayAccess* that) {
    TypePtr atype = attribExpr(that->indexed.get(), env);
    attribExpr(that->index.get(), env, syms.intType);

    if (atype->tag == TypeTags::ARRAY) {
        TypePtr owntype = dynamic_cast<ArrayType*>(atype.get())->elemtype;
        result = owntype;
    } else {
        error("need array type but foung " + atype->tag);
    }
}

void Attr::visitSelect(JCFieldAccess* that) {
}

void Attr::visitLiteral(JCLiteral* that) {
    //must be String
    if (that->typetag == TypeTags::CLASS) {
        result = syms.stringType;
    } else {
        result = syms.typeOfTag[that->typetag];
    }
}

void Attr::visitUnary(JCUnary* that) {
}

//int[2][3][][], dimens={2, 3}, elementType=int[][]
void Attr::visitNewArray(JCNewArray* that) {
    TypePtr elementType;
    if (that->elementType) {
        elementType = attribType(that->elementType.get(), env);
        TypePtr owntype = elementType;
        for (auto iter = that->dimens.begin(); iter != that->dimens.end(); iter++) {
            attribExpr(iter->get(), env, syms.intType);
            owntype = TypePtr(new ArrayType(owntype, syms.arrayClass));
        }
        result = owntype;
    } else {
        //TODO check why
    }
    //doesn't support elems now, i.e. int[] a = new int[]{1, 2, 3}
    //If dimens is not support, then elems is ok, so
    //int[][] arr = new int[2][]{1, 2, 3}, is illegal.
    if (that->elems.size() > 0) {}
}

TypeList Attr::attribArgs(JCExpression::List& trees, Env* env) {
    TypeList argtypes;
    for (auto iter = trees.begin(); iter != trees.end(); iter++) {
        TypePtr type = attribTree(iter->get(), env, Kind::VAL, syms.anyType);
        argtypes.push_back(type);
    }
    return argtypes;
}

