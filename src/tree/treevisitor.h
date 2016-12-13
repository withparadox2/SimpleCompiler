//
// Created by withparadox2 on 2016/11/21.
//

#ifndef SIMPLECOMPILER_TREEVISITOR_H
#define SIMPLECOMPILER_TREEVISITOR_H

#include "alltree.h"

template<typename R, typename P>
class TreeVisitor {
public:
    R visitClassDef(JCClassDecl &that, P &p);

    R visitMethodDef(JCMethodDecl &that, P &p);

    R visitVarDef(JCVariableDecl &that, P &p);

    R visitBlock(JCBlock &that, P &p);

    R visitForLoop(JCForLoop &that, P &p);

    R visitIf(JCIf &that, P &p);

    R visitExec(JCExpressionStatement &that, P &p);

    R visitBreak(JCBreak &that, P &p);

    R visitContinue(JCContinue &that, P &p);

    R visitReturn(JCReturn &that, P &p);

    R visitApply(JCMethodInvocation &that, P &p);

    R visitNewClass(JCNewClass &that, P &p);

    R visitParens(JCParens &that, P &p);

    R visitAssign(JCAssign &that, P &p);

    R visitBinary(JCBinary &that, P &p);

    R visitConditional(JCConditional &that, P &p);

    R visitIndexed(JCArrayAccess &that, P &p);

    R visitSelect(JCFieldAccess &that, P &p);

    R visitIdent(JCIdent &that, P &p);

    template <class T>
    R visitLiteral(JCLiteral<T> &that, P &p);

    R visitTypeIdent(JCPrimitiveTypeTree &that, P &p);

    R visitTypeArray(JCArrayTypeTree &that, P &p);

    R visitModifiers(JCModifiers &that, P &p);
};

#endif //SIMPLECOMPILER_TREEVISITOR_H
