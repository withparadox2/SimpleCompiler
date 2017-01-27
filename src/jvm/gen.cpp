//
// Created by withparadox2 on 2017/1/24.
//

#include "gen.h"
#include "../code/type.h"
#include "Code.h"

#define KEY_GEN "gen"

Gen& Gen::instance() {
    Gen* inst = Context::instance().get<Gen>(KEY_GEN);
    if (inst == nullptr) {
        inst = new Gen();
    }
    return *inst;
}

Gen::Gen() {
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
    localEnv->enclMethod = that->shared_from_this();

    this->pt = that->sym->type->getReturnType();

    //TODO check dimensions

    genMethod(that, localEnv.get(), false);
}

void Gen::genMethod(JCMethodDecl* tree, Env<GenContext>* env, bool fatcode) {
    //Ignore checking params count
    if (tree->body) {
        initCode(tree, env);
        //ignore checking CodeSizeOverflow
        genStat(tree, env);
    }
}

//Assume fatcode = false
void Gen::initCode(JCMethodDecl* tree, Env<GenContext>* env) {
    MethodSymbolPtr meth = tree->sym;
    meth->code = Code::Ptr(new Code);
}

void Gen::genStat(Tree* tree, Env<GenContext>* env) {

}
