//
// Created by withparadox2 on 2017/1/24.
//

#include "gen.h"
#include "../tree/tree.h"

Gen& Gen::instance() {
    static Gen gen;
    return gen;
}

Gen::Gen() {

}

void Gen::genClass(Env<AttrContext>* env, JCClassDecl* cdef) {

}
