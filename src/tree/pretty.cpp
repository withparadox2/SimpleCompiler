//
// Created by withparadox2 on 2016/12/15.
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
    for (auto &def : *that.defs) {
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
    bool isFirst = true;
    for (auto &item : *that.params) {
        if (!isFirst) {
            print(", ");
        }
        isFirst = false;
        item->accept(*this);
    }
    print(")");
    newLine();
    that.body->accept(*this);
}

void Pretty::visitVarDef(JCVariableDecl &that) {
    that.vartype->accept(*this);
    addSpace();
    print(that.name.desc);
    if (that.init != nullptr) {
        that.init->accept(*this);
    }
}

void Pretty::visitBlock(JCBlock &that) {
    print("{");
    newLine();

    for (auto &item : *that.stats) {
        item->accept(*this);
    }
    newLine();
    print("}");
}

void Pretty::visitForLoop(JCForLoop &that) {
    print("for(");
    // For now, don't worry about ',',
    // the init list can has at most one item.
    for (auto &item : *that.init) {
        item->accept(*this);
    }
    print("; ");
    if (that.cond != nullptr) {
        that.cond->accept(*this);
    }
    print("; ");

    for (auto &item : *that.step) {
        item->accept(*this);
    }
    print(")");
    newLine();

    that.body->accept(*this);
}

void Pretty::visitIf(JCIf &that) {
    print("if (");
    that.cond->accept(*this);
    print(") ");
    that.thenPart->accept(*this);
    if (that.elsePart != nullptr) {
        print(" else ");
        that.elsePart->accept(*this);
    }
    newLine();
}

void Pretty::visitExec(JCExpressionStatement &that) {
    that.exp->accept(*this);
}

void Pretty::visitBreak(JCBreak &that) {
    print("break;");
}

void Pretty::visitContinue(JCContinue &that) {
    print("continue");
}

void Pretty::visitReturn(JCReturn &that) {
    print("return ");
    if (that.expr != nullptr) {
        that.expr->accept(*this);
    }
    print(";");
}

void Pretty::visitApply(JCMethodInvocation &that) {
    that.meth->accept(*this);
    print("(");

    bool isFirst = true;
    for (auto &item : *that.args) {
        if (!isFirst) {
            print(", ");
        } else {
            isFirst = false;
        }
        item->accept(*this);
    }
    print(");");
}

void Pretty::visitNewClass(JCNewClass &that) {
    print("new ");
    that.clazz->accept(*this);
    print("(");

    bool isFirst = true;
    for (auto &item : *that.arguments) {
        if (!isFirst) {
            print(", ");
        } else {
            isFirst = false;
        }
        item->accept(*this);
    }
    print(");");
}

void Pretty::visitParens(JCParens &that) {
    print("(");
    if (that.expr != nullptr) {
        that.expr->accept(*this);
    }
    print(")");
}

void Pretty::visitAssign(JCAssign &that) {
    that.lhs->accept(*this);
    print(" = ");
    that.rhs->accept(*this);
}

void Pretty::visitConditional(JCConditional &that) {
    that.cond->accept(*this);
    print(" ? ");
    that.truepart->accept(*this);
    print(" : ");
    that.falsepart->accept(*this);
}

void Pretty::visitBinary(JCBinary &that) {
    that.lhs->accept(*this);
    addSpace();
    print(treeinfo::descByTag(that.treeTag));
    addSpace();
    that.rhs->accept(*this);
}

void Pretty::visitIndexed(JCArrayAccess &that) {
    that.indexed->accept(*this);
    print("[");
    if (that.index != nullptr) {
        that.index->accept(*this);
    }
    print("]");
}

void Pretty::visitSelect(JCFieldAccess &that) {
    print(that.selector.desc);
    print(".");
    that.selected->accept(*this);
}

void Pretty::visitIdent(JCIdent &that) {
    print(that.name.desc);
}

void Pretty::visitLiteral(JCLiteral &that) {
    switch (that.typetag) {
        case TypeTags::BOOLEAN: {
            int val = that.getValue<int>();
            print(val == 1 ? "true" : "false");
            break;
        }
        case TypeTags::INT: {
            int val = that.getValue<int>();
            print("" + val);//todo provide template method?
            break;
        }
        case TypeTags::BOT:
            print("null");
            break;
        case TypeTags::CLASS:
            print(that.getValue<string>());
            break;
        default:
            print("not supported literal " + that.treeTag);
    }

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
    that.elementType->accept(*this);
    print("[]");
}

void Pretty::visitModifiers(JCModifiers &that) {
    printModifiers(that);
}

void Pretty::visitTree(Tree &that) {
    Visitor::visitTree(that);
}

void Pretty::printModifiers(JCModifiers &modifier) {
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
