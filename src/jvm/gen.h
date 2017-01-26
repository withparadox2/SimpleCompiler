//
// Created by withparadox2 on 2017/1/24.
//

#ifndef SIMPLECOMPILER_GEN_H
#define SIMPLECOMPILER_GEN_H

#include <memory>
#include "../tree/alltree.h"
#include "../comp/env.h"

class Visitor;

class GenContext {
public:
    typedef std::shared_ptr<GenContext> Ptr;

};

class Gen : public EnableMapHelper, public Visitor {
private:
    Gen();
    Env<GenContext>* env;
public:
    static Gen& instance();
    void genClass(Env<AttrContext>* env, JCClassDecl* cdef);

    void genDef(Tree* tree, Env<GenContext>* env);

    void visitMethodDef(JCMethodDecl* that) override;
};


#endif //SIMPLECOMPILER_GEN_H
