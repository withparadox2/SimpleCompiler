//
// Created by withparadox2 on 2017/1/8.
//

#ifndef SIMPLECOMPILER_ATTR_H
#define SIMPLECOMPILER_ATTR_H

#include "../code/type.h"
#include "../tree/tree.h"
#include "env.h"

class Attr : public Visitor {
private:
    Kind pKind;
    Env* env;
    Attr();
public:
    static Attr& instance();
    Type::Ptr attribType(Tree* tree, Env* env);
    Type::Ptr attribTree(Tree* tree, Env* env, Kind pkind);

    void visitVarDef(JCVariableDecl& that);
    virtual void visitIdent(JCIdent& that);

};


#endif //SIMPLECOMPILER_ATTR_H
