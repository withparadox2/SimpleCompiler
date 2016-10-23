//
// Created by withparadox2 on 2016/10/23.
//

#ifndef SIMPLECOMPILER_GEN_H
#define SIMPLECOMPILER_GEN_H

#include "Node.h"
#include "Code.h"
#include "Pool.h"

class Gen : public Visitor {
public:
    Gen(Code &code, Pool &pool);

    void visit(ClassNode *node) override;

    void visit(FuncNode *node) override;

    void visit(ModifierNode *node) override;

    void visit(TypeNode *node) override;

    void visit(ParaNode *node) override;

    void visit(InvokeNode *node) override;

    void visit(SelectNode *node) override;

    void visit(StringLiteralNode *node) override;

private:
    Code &code;
    Pool &pool;
};


#endif //SIMPLECOMPILER_GEN_H
