//
// Created by withparadox2 on 2016/11/14.
//

#include "tree.h"
#include "../util/error.h"
#include "visitor.h"
#include "../util/names.h"

JCModifiers::JCModifiers(long long flags)
        : Tree(MODIFIERS), flags(flags) {
}

void JCModifiers::accept(Visitor* visitor) {
    visitor->visitModifiers(this);
}


JCPrimitiveTypeTree::JCPrimitiveTypeTree(int tag)
        : JCExpression(TYPEIDENT), typetag(tag) {
}

void JCPrimitiveTypeTree::accept(Visitor* visitor) {
    visitor->visitTypeIdent(this);
}

JCArrayTypeTree::JCArrayTypeTree(JCExpression* elementType)
        : JCExpression(TYPEARRAY), elementType(elementType) {

}

void JCArrayTypeTree::accept(Visitor* visitor) {
    visitor->visitTypeArray(this);
}

Tree::Tree(int tag) : treeTag(tag) {
}


JCIdent::JCIdent(Name& name) : JCExpression(TYPEIDENT), name(name) {

}

void JCIdent::accept(Visitor* visitor) {
    visitor->visitIdent(this);
}

JCFieldAccess::JCFieldAccess(JCExpression* selected, Name& selector)
        : JCExpression(SELECT), selected(selected),
          selector(selector) {

}

void JCFieldAccess::accept(Visitor* visitor) {
    visitor->visitSelect(this);
}

JCBlock::JCBlock(JCStatement::List& stats)
        : JCStatement(BLOCK), stats(stats) {
}

void JCBlock::accept(Visitor* visitor) {
    visitor->visitBlock(this);
}

JCIf::JCIf(JCExpression* cond, JCStatement* thenpart, JCStatement* elsepart)
        : JCStatement(IF), cond(cond), thenPart(thenpart), elsePart(elsepart) {

}

void JCIf::accept(Visitor* visitor) {
    visitor->visitIf(this);
}

JCExpressionStatement::JCExpressionStatement(JCExpression* exp)
        : JCStatement(EXEC), exp(exp) {

}

void JCExpressionStatement::accept(Visitor* visitor) {
    visitor->visitExec(this);
}

JCForLoop::JCForLoop(JCStatement::List& init,
                     JCExpression* cond,
                     JCExpressionStatement::List step,
                     JCStatement* body)
        : JCStatement(FORLOOP), init(init), cond(cond), step(step), body(body) {

}

void JCForLoop::accept(Visitor* visitor) {
    visitor->visitForLoop(this);
}

JCMethodDecl::JCMethodDecl(JCModifiers* mods,
                           JCExpression* type,
                           Name& name,
                           JCVariableDecl::List& params,
                           JCBlock* body)
        : Tree(METHODDEF), mods(mods), resType(type), name(name), params(params),
          body(body), sym(nullptr) {

}

void JCMethodDecl::accept(Visitor* visitor) {
    visitor->visitMethodDef(this);
}

JCExpression::JCExpression(int tag) : Tree(tag) {

}

JCStatement::JCStatement(int tag) : Tree(tag) {

}

JCClassDecl::JCClassDecl(JCModifiers* mods, Name& name, Tree::List& defs)
        : Tree(CLASSDEF), mods(mods), name(name), defs(defs), sym(nullptr) {
}

void JCClassDecl::accept(Visitor* visitor) {
    visitor->visitClassDef(this);
}

JCBreak::JCBreak() : JCStatement(BREAK) {

}

void JCBreak::accept(Visitor* visitor) {
    visitor->visitBreak(this);
}

JCContinue::JCContinue() : JCStatement(CONTINUE) {

}

void JCContinue::accept(Visitor* visitor) {
    visitor->visitContinue(this);
}

JCReturn::JCReturn(JCExpression* expr) : JCStatement(RETURN), expr(expr) {

}

void JCReturn::accept(Visitor* visitor) {
    visitor->visitReturn(this);
}

JCArrayAccess::JCArrayAccess(JCExpression* indexed, JCExpression* index)
        : JCExpression(INDEXED), indexed(indexed), index(index) {

}

void JCArrayAccess::accept(Visitor* visitor) {
    visitor->visitIndexed(this);
}

JCAssign::JCAssign(JCExpression* lhs, JCExpression* rhs)
        : JCExpression(ASSIGN), lhs(lhs), rhs(rhs) {
}

void JCAssign::accept(Visitor* visitor) {
    visitor->visitAssign(this);
}

int treeinfo::opTag(Token& token) {
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

string treeinfo::descByTag(int treeTag) {
    switch (treeTag) {
        case Tree::OR:
            return "||";
        case Tree::AND:
            return "&&";
        case Tree::EQ:
            return "==";
        case Tree::NE:
            return "!=";
        case Tree::LT:
            return "<";
        case Tree::GT:
            return ">";
        case Tree::LE:
            return "<=";
        case Tree::GE:
            return ">=";
        case Tree::PLUS:
            return "+";
        case Tree::MINUS:
            return "-";
        case Tree::MUL:
            return "*";
        case Tree::DIV:
            return "/";
        case Tree::MOD:
            return "%";
        default:
            return "unknown optag: " + treeTag;
    }
}

bool treeinfo::hasConstructors(const Tree::List& defs) {
    for (auto def : defs) {
        if (isConstructor(*def)) {
            return true;
        }
    }
    return false;
}

bool treeinfo::isConstructor(const Tree& tree) {
    if (tree.treeTag == Tree::METHODDEF) {
        Name& name = dynamic_cast<const JCMethodDecl&>(tree).name;
        return name == *Names::instance().init;
    } else {
        return false;
    }
}

Name* treeinfo::name(Tree* tree) {
    switch (tree->treeTag) {
        case Tree::IDENT:
            return &dynamic_cast<JCIdent*>(tree)->name;
        case Tree::SELECT:
            return &dynamic_cast<JCFieldAccess*>(tree)->selector;
        default:
            return nullptr;
    }
}


JCBinary::JCBinary(int opcode, JCExpression* lhs, JCExpression* rhs)
        : JCExpression(opcode), opcode(opcode), lhs(lhs), rhs(rhs) {
}

void JCBinary::accept(Visitor* visitor) {
    visitor->visitBinary(this);
}

JCConditional::JCConditional(JCExpression* cond, JCExpression* truepart,
                             JCExpression* falsepart)
        : JCExpression(CONDEXPR), cond(cond), truepart(truepart),
          falsepart(falsepart) {

}

void JCConditional::accept(Visitor* visitor) {
    visitor->visitConditional(this);
}

JCParens::JCParens(JCExpression* expr) : JCExpression(PARENS), expr(expr) {
}

void JCParens::accept(Visitor* visitor) {
    visitor->visitParens(this);
}

JCNewArray::JCNewArray(JCExpression* elementType, JCExpression::List& dimens,
                       JCExpression::List& elems)
        : JCExpression(NEWARRAY),
          elementType(elementType),
          dimens(dimens), elems(elems) {
}

void JCNewArray::accept(Visitor* visitor) {
    visitor->visitNewArray(this);
}

JCNewClass::JCNewClass(JCExpression* clazz, JCExpression::List& arguments)
        : JCExpression(NEWCLASS), clazz(clazz), arguments(arguments) {

}

void JCNewClass::accept(Visitor* visitor) {
    visitor->visitNewClass(this);
}

JCMethodInvocation::JCMethodInvocation(JCExpression::List& args, JCExpression* meth)
        : JCExpression(APPLY), args(args), meth(meth) {
}

void JCMethodInvocation::accept(Visitor* visitor) {
    visitor->visitApply(this);
}

JCVariableDecl::JCVariableDecl(Name& name, JCExpression* vartype)
        : JCStatement(VARDEF), name(name), vartype(vartype), init(nullptr) {

}

JCVariableDecl::JCVariableDecl(Name& name, JCExpression* vartype, JCExpression* init)
        : JCStatement(VARDEF), name(name), vartype(vartype), init(init) {

}

void JCVariableDecl::accept(Visitor* visitor) {
    visitor->visitVarDef(this);
}

JCUnary::JCUnary(int opcode, JCExpression* arg)
        : JCExpression(opcode), opcode(opcode), arg(arg) {

}

void JCUnary::accept(Visitor* visitor) {
    visitor->visitUnary(this);
}


void JCLiteral::accept(Visitor* visitor) {
    visitor->visitLiteral(this);
}

JCLiteral::~JCLiteral() {
    delete value;
}
