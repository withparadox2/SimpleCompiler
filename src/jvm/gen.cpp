//
// Created by withparadox2 on 2017/1/24.
//

#include <memory>
#include "gen.h"
#include "../code/type.h"
#include "../code/Flags.h"
#include "../code/Symtab.h"

#define KEY_GEN "gen"

Gen& Gen::instance() {
    Gen* inst = Context::instance().get<Gen>(KEY_GEN);
    if (inst == nullptr) {
        inst = new Gen();
    }
    return *inst;
}

Gen::Gen() : pool(new Pool),
             syms(Symtab::instance()),
             names(Names::instance()) {
    Context::instance().put(KEY_GEN, this);
}

void Gen::genClass(Env<AttrContext>* env, JCClassDecl* cdef) {
    Env<GenContext>::Ptr localEnv(
            new Env<GenContext>(
                    cdef->shared_from_this(),
                    GenContext::Ptr(new GenContext)
            ));
    for (auto iter = cdef->defs.begin();
         iter != cdef->defs.end(); iter++) {
        genDef(iter->get(), localEnv.get());
    }
}

void Gen::genDef(Tree* tree, Env<GenContext>* env) {
    Env<GenContext>* preEnv = this->env;
    this->env = env;
    tree->accept(this);
    this->env = preEnv;
}

void Gen::visitMethodDef(JCMethodDecl* that) {
    Env<GenContext>::Ptr localEnv(env->dup(that));
//    localEnv->enclMethod =  that->shared_from_this();

    this->pt = that->sym->type->getReturnType();

    //TODO check dimensions

    genMethod(that, localEnv.get(), false);
}

void Gen::genMethod(JCMethodDecl* tree, Env<GenContext>* env, bool fatcode) {
    //Ignore checking params count
    if (tree->body) {
        initCode(tree, env);
        //Ignore checking CodeSizeOverflow
        genStat(tree, env);
    }
}

//Assume fatcode = false
void Gen::initCode(JCMethodDecl* tree, Env<GenContext>* env) {
    MethodSymbolPtr meth = tree->sym;
    //TODO construct code
    meth->code = Code::Ptr(new Code);
    this->code = meth->code;

    items = Items::Ptr(new Items(code, pool));

    //For non static method, insert `this` as the
    //first parameter.
    if ((tree->mods->flags & Flags::STATIC) == 0) {
        TypePtr selfType = meth->owner->type;

        if (meth->isConstructor() && selfType != syms.objectType) {
            //TODO UninitializedType
        }

        this->code->setDefined(
                this->code->newLocal(
                        VarSymbolPtr(new VarSymbol(Flags::FINAL,
                                                   *names._this,
                                                   selfType,
                                                   meth->owner))));

        for (auto iter = tree->params.begin();
             iter != tree->params.end();
             iter++) {
            this->code->setDefined(
                    this->code->newLocal(iter->get()->sym));
        }

        //TODO entryPoint
    }

}

void Gen::genStat(Tree* tree, Env<GenContext>* env) {
    genDef(tree, env);
}

void Gen::visitVarDef(JCVariableDecl* that) {
    VarSymbolPtr v = that->sym;
    code->newLocal(v);
    if (that->init) {
        //TODO check init expr tobe const value
        genExpr(that->init.get(), v->type)->load();
    }
}

void Gen::visitBlock(JCBlock* that) {
}

void Gen::visitForLoop(JCForLoop* that) {
}

void Gen::visitIf(JCIf* that) {
}

void Gen::visitExec(JCExpressionStatement* that) {
}

void Gen::visitBreak(JCBreak* that) {
}

void Gen::visitContinue(JCContinue* that) {
}

void Gen::visitReturn(JCReturn* that) {
}

void Gen::visitApply(JCMethodInvocation* that) {
}

void Gen::visitNewClass(JCNewClass* that) {
}

void Gen::visitParens(JCParens* that) {
}

void Gen::visitAssign(JCAssign* that) {
}

void Gen::visitConditional(JCConditional* that) {
}

void Gen::visitBinary(JCBinary* that) {
}

void Gen::visitIndexed(JCArrayAccess* that) {
}

void Gen::visitSelect(JCFieldAccess* that) {
}

void Gen::visitIdent(JCIdent* that) {
}

void Gen::visitLiteral(JCLiteral* that) {
}

void Gen::visitTypeIdent(JCPrimitiveTypeTree* that) {
}

void Gen::visitTypeArray(JCArrayTypeTree* that) {
}

void Gen::visitUnary(JCUnary* that) {
}

void Gen::visitNewArray(JCNewArray* that) {
}

Item::Ptr Gen::genExpr(Tree* tree, TypePtr ptr) {
    TypePtr prePt = this->pt;
    this->pt = ptr;
    tree->accept(this);
    this->pt = prePt;
    return result;
}
