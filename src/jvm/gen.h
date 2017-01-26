//
// Created by withparadox2 on 2017/1/24.
//

#ifndef SIMPLECOMPILER_GEN_H
#define SIMPLECOMPILER_GEN_H

#include "../tree/alltree.h"
#include "../comp/env.h"

class Gen : public EnableMapHelper {
private:
    Gen();
public:
    static Gen& instance();
    void genClass(Env<AttrContext>* env, JCClassDecl* cdef);
};


#endif //SIMPLECOMPILER_GEN_H
