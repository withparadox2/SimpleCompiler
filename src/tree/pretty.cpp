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
    indent();
    newLine();

    auto begin = that.defs.begin();
    auto end = that.defs.end();
    for (auto item = begin; item != end; ++item) {
        (*item)->accept(*this);
        if (item + 1 == end) {
            undent();
        }
        newLine();
    }

    print("}");
}

void Pretty::visitMethodDef(JCMethodDecl &that) {
    if (that.mods != nullptr) {
        that.mods->accept(*this);
    }

    that.type->accept(*this);
    addSpace();
    print(that.name.desc);
    addSpace();

    printTreeList(that.params, ",", "(", ") ");
    that.body->accept(*this);
}

void Pretty::visitVarDef(JCVariableDecl &that) {
    that.vartype->accept(*this);
    addSpace();
    print(that.name.desc);
    if (that.init != nullptr) {
        print(" = ");
        that.init->accept(*this);
    }
}

void Pretty::visitBlock(JCBlock &that) {
    print("{");
    indent();
    newLine();

    auto begin = that.stats.begin();
    auto end = that.stats.end();
    for (auto item = begin; item != end; ++item) {
        (*item)->accept(*this);
        if (item + 1 == end) {
            undent();
        }
        newLine();
    }
    print("}");
}

void Pretty::visitForLoop(JCForLoop &that) {
    print("for(");
    // For now, don't worry about ',',
    // the init list only has one item.
    for (auto &item : that.init) {
        item->accept(*this);
    }
    print("; ");
    if (that.cond != nullptr) {
        that.cond->accept(*this);
    }
    print("; ");

    for (auto &item : that.step) {
        item->accept(*this);
    }
    print(") ");

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
}

void Pretty::visitExec(JCExpressionStatement &that) {
    that.exp->accept(*this);
}

void Pretty::visitBreak(JCBreak &that) {
    print("break");
}

void Pretty::visitContinue(JCContinue &that) {
    print("continue");
}

void Pretty::visitReturn(JCReturn &that) {
    print("return");
    if (that.expr != nullptr) {
        addSpace();
        that.expr->accept(*this);
    }
}

void Pretty::visitApply(JCMethodInvocation &that) {
    that.meth->accept(*this);

    printTreeList(that.args, ",", "(", ")");
}

void Pretty::visitNewClass(JCNewClass &that) {
    print("new ");
    that.clazz->accept(*this);
    printTreeList(that.arguments, ",", "(", ")");
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
    that.selected->accept(*this);
    print(".");
    print(that.selector.desc);
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
            print(std::to_string(val));
            break;
        }
        case TypeTags::BOT:
            print("null");
            break;
        case TypeTags::CLASS:
            print("\"");
            print(that.getValue<string>());
            print("\"");
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
    long long flags = modifier.flags;
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

void Pretty::visitNewArray(JCNewArray &that) {
    //todo handle dimens
    print("new ");
    that.elementType->accept(*this);
    print("[]");
    printTreeList(that.elems, ",", " {", "}");
}


void Pretty::visitMethodInvocation(JCMethodInvocation &that) {
    that.meth->accept(*this);
    printTreeList(that.args, ",", " {", "}");
}


void Pretty::print(const string &str) {
    cout << str;
}

void Pretty::addSpace() {
    print(" ");
}

void Pretty::newLine() {
    print("\n");
    for (int i = 0; i < tabCount; i++) {
        print("    ");
    }
}

Pretty::Pretty() : tabCount(0) {

}

void Pretty::indent() {
    tabCount++;
}

void Pretty::undent() {
    tabCount--;
    if (tabCount < 0) {
        tabCount = 0;
    }
}

template<typename T>
void Pretty::printTreeList(vector<T> &list, string split, string wLeft, string wRight) {
    print(wLeft);
    bool isFirst = true;
    for (auto &item : list) {
        if (!isFirst) {
            print(split);
            addSpace();
        } else {
            isFirst = false;
        }
        item->accept(*this);
    }
    print(wRight);
}

