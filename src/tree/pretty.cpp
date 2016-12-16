//
// Created by ruancongyong on 2016/12/15.
//

#include "pretty.h"
#include "../code/Flags.h"

void Pretty::visitClassDef(JCClassDecl &that) {
    if (that.mods != nullptr) {
        that.mods->accept(*this);
    }
    print("class ");
    print(that.name.desc);
    print(" {");
    newLine();
    for(auto &def : *that.defs) {
        def->accept(*this);
    }
    print("}");
}

void Pretty::visitMethodDef(JCMethodDecl &that) {
    if (that.mods != nullptr) {
        that.mods->accept(*this);
    }

    that.type->accept(*this);
    addSpace();
    print(that.name->desc);
    addSpace();
    print("(");
    for (auto &item : *that.params) {
        item->accept(*this);
    }
}

void Pretty::visitVarDef(JCVariableDecl &that) {
    Visitor::visitVarDef(that);
}

void Pretty::visitBlock(JCBlock &that) {
    Visitor::visitBlock(that);
}

void Pretty::visitForLoop(JCForLoop &that) {
    Visitor::visitForLoop(that);
}

void Pretty::visitIf(JCIf &that) {
    Visitor::visitIf(that);
}

void Pretty::visitExec(JCExpressionStatement &that) {
    Visitor::visitExec(that);
}

void Pretty::visitBreak(JCBreak &that) {
    Visitor::visitBreak(that);
}

void Pretty::visitContinue(JCContinue &that) {
    Visitor::visitContinue(that);
}

void Pretty::visitReturn(JCReturn &that) {
    Visitor::visitReturn(that);
}

void Pretty::visitApply(JCMethodInvocation &that) {
    Visitor::visitApply(that);
}

void Pretty::visitNewClass(JCNewClass &that) {
    Visitor::visitNewClass(that);
}

void Pretty::visitParens(JCParens &that) {
    Visitor::visitParens(that);
}

void Pretty::visitAssign(JCAssign &that) {
    Visitor::visitAssign(that);
}

void Pretty::visitConditional(JCConditional &that) {
    Visitor::visitConditional(that);
}

void Pretty::visitBinary(JCBinary &that) {
    Visitor::visitBinary(that);
}

void Pretty::visitIndexed(JCArrayAccess &that) {
    Visitor::visitIndexed(that);
}

void Pretty::visitSelect(JCFieldAccess &that) {
    Visitor::visitSelect(that);
}

void Pretty::visitIdent(JCIdent &that) {
    Visitor::visitIdent(that);
}

void Pretty::visitLiteral(JCLiteral &that) {
    Visitor::visitLiteral(that);
}

void Pretty::visitTypeIdent(JCPrimitiveTypeTree &that) {
    switch (that.typetag) {
        case TypeTags::INT:
            print("int");
            break;
        case TypeTags::BOOLEAN:
            print("boolean");
            break;
        case TypeTags::BOT:
            print("null");
            break;
        case TypeTags::VOID:
            print("void");
            break;
        default:
            print("type not supported:" + that.typetag);
            break;
    }
}

void Pretty::visitTypeArray(JCArrayTypeTree &that) {
    Visitor::visitTypeArray(that);
}

void Pretty::visitModifiers(JCModifiers &that) {
    printModifiers(that);
}

void Pretty::visitTree(Tree &that) {
    Visitor::visitTree(that);
}

void Pretty::printModifiers(JCModifiers& modifier) {
    int flags = modifier.flags;
    if ((flags & Flags::PUBLIC) != 0) {
        print("public");
        addSpace();
    }
    if ((flags & Flags::PRIVATE) != 0) {
        print("private");
        addSpace();
    }
    if ((flags & Flags::STATIC) != 0) {
        print("static");
        addSpace();
    }
}

void Pretty::print(const string &str) {
    cout << str;
}

void Pretty::addSpace() {
    print(" ");
}

void Pretty::newLine() {
    print("\n");
}

//void Pretty::printType(JCExpression *type) {
//    switch (type->treeTag) {
//        case Tree:: :
//    }
//}
