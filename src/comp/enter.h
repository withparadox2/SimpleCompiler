//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_ENTER_H
#define SIMPLECOMPILER_ENTER_H

#include "../tree/tree.h"
#include "../code/symbol.h"
#include "../util/names.h"

class Enter : public Visitor {
private:
public:
    static Enter &instance();
    void complete(Tree *tree);

    void visitClassDef(JCClassDecl &that) override;

    virtual void visitMethodDef(JCMethodDecl &that) override;

    virtual void visitTree(Tree &that) override;

};


#endif //SIMPLECOMPILER_ENTER_H
