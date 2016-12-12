//
// Created by withparadox2 on 2016/11/14.
//

#include "tree.h"
#include "../util/error.h"

JCModifiers::JCModifiers(int flags) : Tree(MODIFIERS), flags(flags) {
}


JCPrimitiveTypeTree::JCPrimitiveTypeTree(int tag) : JCExpression(TYPEIDENT), typetag(tag) {
}

JCArrayTypeTree::JCArrayTypeTree(JCExpression *elementType) : JCExpression(TYPEARRAY), elementType(elementType) {

}

Tree::Tree(int tag) : treeTag(tag) {
}

JCIdent::JCIdent(Name &name) : JCExpression(TYPEIDENT), name(name) {

}

JCFieldAccess::JCFieldAccess(JCExpression *selected, Name &selector) : JCExpression(SELECT), selected(selected),
                                                                       selector(selector) {

}

JCVariableDecl::JCVariableDecl(JCExpression *type, Name &name) : Tree(VARDEF), type(type), name(name) {

}

JCBlock::JCBlock(vector<JCStatement *> *stats) : JCStatement(BLOCK), stats(stats) {

}

JCIf::JCIf(JCExpression *cond, JCStatement *thenpart, JCStatement *elsepart) : JCStatement(IF), cond(cond),
                                                                               thenPart(thenpart),
                                                                               elsePart(elsepart) {

}

JCExpressionStatement::JCExpressionStatement(JCExpression *exp) : JCStatement(EXEC), exp(exp) {

}

JCForLoop::JCForLoop(vector<JCStatement *> *init, JCExpression *cond, vector<JCExpressionStatement *> *step,
                     JCStatement *body) : JCStatement(FORLOOP), init(init), cond(cond), step(step), body(body) {

}

JCMethodDecl::JCMethodDecl(JCModifiers *mods, JCExpression *type, Name *name, vector<JCExpression *> *params,
                           JCBlock *body) : Tree(METHODDEF), mods(mods), type(type), name(name), params(params),
                                            body(body) {

}

JCExpression::JCExpression(int tag) : Tree(tag) {

}

JCStatement::JCStatement(int tag) : Tree(tag) {

}

JCClassDecl::JCClassDecl(JCModifiers *mods, Name &name, vector<Tree *> *defs) : Tree(CLASSDEF), mods(mods), name(name),
                                                                                defs(defs) {

}

JCBreak::JCBreak() : JCStatement(BREAK) {

}

JCContinue::JCContinue() : JCStatement(CONTINUE) {

}

JCReturn::JCReturn(JCExpression *expr) : JCStatement(RETURN), expr(expr) {

}

JCArrayAccess::JCArrayAccess(JCExpression *indexed, JCExpression *index) : JCExpression(INDEXED), indexed(indexed),
                                                                           index(index) {

}

JCAssign::JCAssign(JCExpression *lhs, JCExpression *rhs) : JCExpression(ASSIGN), lhs(lhs), rhs(rhs) {

}

int treeinfo::opTag(Token &token) {
    switch (token.id) {
        case Token::ID_BARBAR:
            return Tree::OR;
        case Token::ID_AMPAMP:
            return Tree::AND;
        case Token::ID_EQEQ:
            return Tree::EQ;
        case Token::ID_BANGEQ:
            return Tree::NE;
        case Token::ID_GT:
            return Tree::GT;
        case Token::ID_LT:
            return Tree::LT;
        case Token::ID_LTEQ:
            return Tree::LE;
        case Token::ID_GTEQ:
            return Tree::GE;
        case Token::ID_PLUS:
            return Tree::PLUS;
        case Token::ID_SUB:
            return Tree::MINUS;
        case Token::ID_STAR:
            return Tree::MUL;
        case Token::ID_SLASH:
            return Tree::DIV;
        case Token::ID_PERCENT:
            return Tree::MOD;
        default:
            return -1;
    }
}

int treeinfo::opPrec(int op) {
    switch (op) {
        case Tree::POS:
        case Tree::NEG:
        case Tree::NOT:
        case Tree::COMPL:
            return prefixPrec;
        case Tree::ASSIGN:
            return assignPrec;
        case Tree::OR:
            return orPrec;
        case Tree::AND:
            return andPrec;
        case Tree::EQ:
        case Tree::NE:
            return eqPrec;
        case Tree::LT:
        case Tree::LE:
        case Tree::GT:
        case Tree::GE:
            return ordPrec;
        case Tree::PLUS:
        case Tree::MINUS:
            return addPrec;
        case Tree::MUL:
        case Tree::DIV:
        case Tree::MOD:
            return mulPrec;
        default:
            error("can not cal prec for op " + op);
    }
    return -1;
}

JCBinary::JCBinary(int opcode, JCExpression *lhs, JCExpression *rhs) : JCExpression(opcode), opcode(opcode), lhs(lhs), rhs(rhs) {
}

JCConditional::JCConditional(JCExpression *cond, JCExpression *truepart, JCExpression *flasepart) : JCExpression(CONTINUE) {

}

JCParens::JCParens(JCExpression *expr) : JCExpression(PARENS), expr(expr) {
}

JCNewArray::JCNewArray(JCExpression *elementType, vector<JCExpression *> *dimens) : JCExpression(NEWARRAY), elementType(elementType), dimens(dimens) {
}
