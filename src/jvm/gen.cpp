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

}
