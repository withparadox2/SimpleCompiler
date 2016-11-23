//
// Created by withparadox2 on 2016/11/14.
//

#ifndef SIMPLECOMPILER_TREE_H
#define SIMPLECOMPILER_TREE_H

#include <vector>
#include "treevisitor.h"

using std::vector;

class Tree {
public:
    int tag;

    //Type* type;
    virtual void accept(Visitor &v) = 0;
    template <typename R, typename D>
    R accept(TreeVisitor<R, D> v, D& d);
};

class JCClassDecl : public Tree {
public:
    Name &name;
    vector<JCTree*> defs;
//    ClassSymbol sym;
};

class JCExpression : public Tree {

};

class JCStatement : public Tree {

};

class JCMethodDecl : public Tree {

};

class JCVariableDecl : public Tree {

};

class JCBlock : public JCStatement {

};

class JCForLoop : public JCStatement {

};

class JCIf : public JCStatement {

};

class JCExpressionStatement : public JCStatement {

};

class JCBreak : public JCStatement {

};

class JCContinue : public JCStatement {

};

class JCReturn : public JCStatement {

};

class JCMethodInvocation : public JCExpression {

};

class JCNewClass : public JCExpression {

};

class JCParens : public JCExpression {

};

class JCAssign : public JCExpression {

};

class JCBinary : public JCExpression {

};

class JCArrayAccess : public JCExpression {

};

class JCFieldAccess : public JCExpression {

};

class JCIdent : public JCExpression {

};

class JCLiteral : public JCExpression {

};

class JCPrimitiveTypeTree : public JCExpression {

};

class JCArrayTypeTree : public JCExpression {

};

class JCModifiers : public Tree {

};

class Visitor {
public:
    void visitClassDef(JCClassDecl &that) { visitTree(that); }

    void visitMethodDef(JCMethodDecl &that) { visitTree(that); }

    void visitVarDef(JCVariableDecl &that) { visitTree(that); }

    void visitBlock(JCBlock &that) { visitTree(that); }

    void visitForLoop(JCForLoop &that) { visitTree(that); }

    void visitIf(JCIf &that) { visitTree(that); }

    void visitExec(JCExpressionStatement &that) { visitTree(that); }

    void visitBreak(JCBreak &that) { visitTree(that); }

    void visitContinue(JCContinue &that) { visitTree(that); }

    void visitReturn(JCReturn &that) { visitTree(that); }

    void visitApply(JCMethodInvocation &that) { visitTree(that); }

    void visitNewClass(JCNewClass &that) { visitTree(that); }

    void visitParens(JCParens &that) { visitTree(that); }

    void visitAssign(JCAssign &that) { visitTree(that); }

    void visitBinary(JCBinary &that) { visitTree(that); }

    void visitIndexed(JCArrayAccess &that) { visitTree(that); }

    void visitSelect(JCFieldAccess &that) { visitTree(that); }

    void visitIdent(JCIdent &that) { visitTree(that); }

    void visitLiteral(JCLiteral &that) { visitTree(that); }

    void visitTypeIdent(JCPrimitiveTypeTree &that) { visitTree(that); }

    void visitTypeArray(JCArrayTypeTree &that) { visitTree(that); }

    void visitModifiers(JCModifiers &that) { visitTree(that); }

    void visitTree(JCTree &that) {
        //TODO error
    }
};

#endif //SIMPLECOMPILER_TREE_H
