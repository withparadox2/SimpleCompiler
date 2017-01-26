//
// Created by withparadox2 on 2017/1/24.
//

#include "gen.h"

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


}
