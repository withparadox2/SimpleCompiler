//
// Created by withparadox2 on 2017/1/8.
//

#include <iostream>
#include "attr.h"
#include "../util/log.h"
#include "../code/Symtab.h"
#include "enter.h"
#include "../util/error.h"
#include "../util/tools.h"

Attr& Attr::instance() {
    static Attr attr;
    return attr;
}

TypePtr Attr::attribType(Tree* tree, Env* env) {
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


TypePtr Attr::attribTree(Tree* tree, Env* env, int pkind, TypePtr pt) {
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
    if (env->info->scope->owner.lock()->kind == Kind::MTH) {
        if (that->sym) {
            env->info->scope->enter(that->sym);
        } else {
            enter().complete(that, env);
        }
    }
    VarSymbolPtr v = that->sym;
    that->type = v->type;
    result = that->type;
}

void Attr::visitIdent(JCIdent* that) {
    SymbolPtr sym;

    //see visitApply
    if (pt->tag == TypeTags::METHOD) {
        sym = findMethod(env, env->enclClass->sym->type,
                         that->name, pt->getParameterTypes(), false);
    } else if (that->sym && that->sym->kind != Kind::VAR) {
        sym = that->sym;
    } else {
        sym = SymbolPtr(resolveIdent(env, that->name, pKind));
    }
    that->sym = sym;

    if (!sym) {
        error("Fail to find symbol " + that->name.desc);
    }

    result = selectType(sym);
}

void Attr::visitTypeArray(JCArrayTypeTree* that) {
    TypePtr eType = attribType(that->elementType.get(), this->env);
    result = TypePtr(new ArrayType(eType, syms.arrayClass));
    that->type = result;
}

SymbolPtr Attr::resolveIdent(Env* env, const Name& name, int kind) {
    using namespace Kind;
    SymbolPtr bestSoFar = syms.noSymbol;

    if ((kind & VAR) != 0) {
        bestSoFar = findVar(env, name);
        if (bestSoFar) {
            return bestSoFar;
        }
    }

    if ((kind & TYP) != 0) {
        bestSoFar = findType(env, name);
        if (bestSoFar) {
            return bestSoFar;
        }
    }

    return bestSoFar;
}

SymbolPtr Attr::findType(Env* env, const Name& name) {
    for (Env* env1 = env; env1->outer; env1 = env1->outer.get()) {
        SymbolPtr sym = env1->info->scope->lookUp(name);
        if (sym && sym->kind == Kind::TYP) {
            return sym;
        }
    }

    StarImportScope& gScope = StarImportScope::instance();

    SymbolPtr sym = gScope.lookUp(name);
    if (sym) {
        log("find type : " + sym->name.desc);
        return sym;
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

void Attr::attribClass(ClassSymbolPtr c) {
    Env* env = enter().typeEnvs.at(c);
    JCClassDecl::Ptr tree = env->enclClass;
    for (auto iter = tree->defs.begin(); iter != tree->defs.end(); iter++) {
        attribStat(iter->get(), env);
    }
}

void Attr::visitMethodDef(JCMethodDecl* that) {
    log("attr method : " + that->name.desc);
    //ignore any check.
    MethodSymbolPtr m = that->sym;
    JCMethodDecl::Ptr treePtr = std::dynamic_pointer_cast<JCMethodDecl>
            (that->shared_from_this());
    Env::Ptr localEnv(enter().methodEnv(treePtr, env));

    // Attribute all value parameters.
    attribStats(that->params, localEnv.get());

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
    loopEnv->info->scope->printTable();
    attribStats(that->init, loopEnv.get());
    if (that->cond) {
        attribExpr(that->cond.get(), loopEnv.get(), syms.booleanType);
    }
    // before, we were not in loop!
    loopEnv->tree = that->shared_from_this();
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

void Attr::visitApply(JCMethodInvocation* that) {
    Env::Ptr localEnv(env->dup(that, env->info->dup()));
    Name* methName = treeinfo::name(that->meth.get());
    bool isConstructor =
            methName == names._this || methName == names._super;
    if (isConstructor) {
        //We are seeing ...this(...) or ...super(...)
        localEnv->info->isSelfCall = true;
        TypeList argtypes = attribArgs(that->args, localEnv.get());

        //Point to the class in which the called constructor is defined.
        //If the expr is this(...), then site is the type of enclClass,
        //If the expr is super(...), then site is the type of superClass.
        TypePtr site = env->enclClass->sym->type;

        if (methName == names._super) {
            if (site == syms.objectType) {
                error("Object doesn't have a super class.");
            } else {
                site = dynamic_pointer_cast<ClassType>(site)->supertype_field;
            }
        }

        //Actually, it can be nothing else.
        if (site->tag == TypeTags::CLASS) {
            //Don't support inner class, ignore enclosing type.
            if (that->meth->treeTag == Tree::SELECT) {
                error("class " + env->enclClass->sym->name.desc + " is not a inner class.");
            }

            // Ignore selectSuper, which is used to check accessibility of selector,
            // such as super.methodName()
            SymbolPtr sym = resolveConstructor(localEnv.get(), site, argtypes);

            treeinfo::setSymbol(that, sym);

            //Ignore checkId()
        }

        that->type = syms.voidType;
        result = syms.voidType;
    } else {
        TypeList argtypes = attribArgs(that->args, localEnv.get());

        TypePtr tempType = newMethTemplate(argtypes);

        //We expect mType to be tempType while resolving expr 'meth', which may
        //be an ident or a FieldAccess expr.
        TypePtr mType = attribExpr(that->meth.get(), localEnv.get(), tempType);

        that->type = mType->getReturnType();
        //The type of a method invocation must be the return type, so assign it.
        result = that->type;
    }
}

void Attr::visitNewClass(JCNewClass* that) {
    Env* localEnv = env->dup(that, env->info->dup());
    JCExpression::Ptr clazz = that->clazz;

    TypePtr clazztype = attribType(clazz.get(), env);

    TypeList argtypes = attribArgs(that->arguments, localEnv);
    that->type = clazztype;
    result = that->type;
}

void Attr::visitParens(JCParens* that) {
    // Why env, pKind, pt?
    // If int type is expected, the expr must be an int type,
    // since paren itself doesn't own a type, all information
    // is stored in JCParens.expr.
    that->type = attribTree(that->expr.get(), env, pKind, pt);
    result = that->type;
}

void Attr::visitAssign(JCAssign* that) {
    TypePtr owntype = attribTree(that->lhs.get(), env->dup(that), Kind::VAR, syms.noType);
    //Ignore capture type

    // We expect that right type is a sub type of left type.
    attribExpr(that->rhs.get(), env, owntype);
    result = owntype;
}

void Attr::visitConditional(JCConditional* that) {
    attribExpr(that->cond.get(), env, syms.booleanType);
    attribExpr(that->truepart.get(), env);
    attribExpr(that->falsepart.get(), env);
    // Assume types is correct, which means true part's
    // type is same as right part's type
    result = that->truepart->type;
}

void Attr::visitBinary(JCBinary* that) {
    TypePtr left = attribExpr(that->lhs.get(), env);
    TypePtr right = attribExpr(that->rhs.get(), env);

    that->sym = resolveBinaryOperator(that->opcode, env, left, right);
    result = that->sym->type->getReturnType();
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
    //Example A.B.C
    using namespace Kind;
    int skind = 0;
    if (that->selector == *names._this
        || that->selector == *names._super
        || that->selector == *names._class) {
        skind = TYP;
    } else {
        // If C is a type, A.B must also be a type
        if ((pKind & TYP) != 0) {
            skind = skind | TYP;
        }

        // If C is a variable or method, then A.B
        // must be a class type (and C is static),
        // or an instance of a class (e.g. System.out).
        if ((pKind & (VAL | MTH)) != 0) {
            skind = skind | VAL | TYP;
        }
    }

    TypePtr site = attribTree(that->selected.get(), env, skind, syms.anyType);

    //Ignore capture type.
    //Ignore check T.class and T[].class, etc

    //Symbol of A.B, we will search symbol of C in sitesym.
    SymbolPtr sitesym = treeinfo::symbol(that->selected.get());

    //Ignore selectSuper

    SymbolPtr sym = selectSym(that, sitesym, site, env, pt, pKind);

    log("select sym " + sym->name.desc);
    that->sym = sym;
    that->type = selectType(sym);
    result = that->type;
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
    //Only support +expr, -expr, then attribution can be simple.
    //If ++_ or --_ or _++ or _-- then we should attrib VAR, we
    //are only allowed to apply these on a variable.
    TypePtr argType = attribExpr(that->arg.get(), env);

    that->sym = resolveUnaryOperator(that->opcode, env, argType);
    result = that->sym->type->getReturnType();
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

SymbolPtr Attr::resolveUnaryOperator(int optag, Env* env, TypePtr arg) {
    resolveOperator(optag, env, ofList(arg));
}

SymbolPtr Attr::resolveBinaryOperator(int optag, Env* env, TypePtr left, TypePtr right) {
    return resolveOperator(optag, env, ofList(left, right));
}

SymbolPtr Attr::resolveOperator(int optag, Env* env, TypeList argtypes) {
    Name& name = treeinfo::operatorName(optag);
    return findMethod(env, syms.predefClass->type,
                      name, argtypes, true);
}

SymbolPtr Attr::findMethod(Env* env, TypePtr site, const Name& name, TypeList argTypes, bool isOperator) {
    //Simplified, without checking, chosing
    ClassSymbolPtr sym = dynamic_pointer_cast<ClassSymbol>(site->tsym.lock());
    SymbolPtr result = sym->member()->lookUp(name);

    if (result) {
        log("find method of " + name.desc + ":" + result->name.desc);
    } else {
        log("not find method of " + name.desc);
    }
    return result;
}

TypePtr Attr::newMethTemplate(TypeList argtypes) {
    MethodTypePtr mt(new MethodType(argtypes, TypePtr(nullptr), syms.methodClass));
    return mt;
}

SymbolPtr Attr::resolveConstructor(Env* env, TypePtr site, TypeList argtypes) {
    return findMethod(env, site, *names.init, argtypes, false);
}

SymbolPtr Attr::selectSym(JCFieldAccess* tree, SymbolPtr sitesym, TypePtr site, Env* env, TypePtr pt, int pkind) {
    Name& name = tree->selector;
    switch (site->tag) {
        case TypeTags::ARRAY:
        case TypeTags::CLASS:
            if (pt->tag == TypeTags::METHOD) {
                return findMethod(env, site, name, pt->getParameterTypes(), false);
            } else if (name == *names._this || name == *names._super) {
                if (sitesym == env->enclClass->sym) {
                    return env->info->scope->lookUp(name);
                }
            } else {
                //Assume it is a field
                return site->tsym.lock()->member()->lookUp(name);
            }
    }
    return syms.noSymbol;
}

//See javac checkId(), we have simpified this too much...
TypePtr Attr::selectType(SymbolPtr sym) {
    switch (sym->kind) {
        case Kind::TYP:
            return sym->type;
        case Kind::MTH:
            return sym->type->getReturnType();
        case Kind::VAR:
            return sym->type;
        default:
            error("unexpeted type");
    }
}

SymbolPtr Attr::findVar(Env* env, const Name& name) {
    SymbolPtr result = env->info->scope->lookUp(name);
    if (result) {
        return result;
    }

    result = env->enclClass->sym->member()->lookUp(name);
    if (result) {
        return result;
    }

    StarImportScope& gScope = StarImportScope::instance();

    SymbolPtr sym = gScope.lookUp(name);

    return sym;
}

SymbolPtr Attr::findField(Env* env, const Name& name, TypePtr site, SymbolPtr c) {
    return SymbolPtr();
}


