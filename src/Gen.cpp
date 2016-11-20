//
// Created by withparadox2 on 2016/10/23.
//

#include "Gen.h"

Gen::Gen(Code &code, Pool &pool) : code(code), pool(pool) {
}

void Gen::visit(FuncNode *node) {
    code.w2B(code.buildModifierFlag(node->modifierNodes));
    code.w2B(pool.genUtf8(node->name)->index + 1);//name_index
    code.w2B(pool.genUtf8(node->buildDescriptor())->index + 1);//descriptor
    code.w2B(1);//attribute count
    code.w2B(pool.genUtf8("Code")->index + 1);

    if (node->name == "<init>") {
        code.w4B(17);
        code.w2B(1);
        code.w2B(1);
        code.w4B(5);//code length
        code.w1B(0x2a);//aload this
        code.w1B(0xb7);//invoke special

        int superFuncIndex = node->statementNodes[0]->selectNode->genConstantRef(pool, code.getRootNode())->index + 1;
        code.w2B(superFuncIndex);
        code.w1B(0xb1);//return
    } else {
        SelectNode *selectNode = node->statementNodes[0]->selectNode;
        code.w4B(21);
        code.w2B(2);
        code.w2B(1);
        code.w4B(9);
        code.w1B(0xb2);
        code.w2B(selectNode->next->genConstantRef(pool, code.getRootNode())->index + 1);
        code.w1B(0x12);
        code.w1B(pool.genString(node->statementNodes[0]->argNode->value)->index + 1);
        code.w1B(0xb6);
        code.w2B(selectNode->next->next->genConstantRef(pool, code.getRootNode())->index + 1);
        code.w1B(0xb1);
    }
    code.w2B(0);//exception table length
    code.w2B(0);//attribute count
}

void Gen::visit(ClassNode *node) {};

void Gen::visit(ModifierNode *node) {};

void Gen::visit(TypeNode *node) {};

void Gen::visit(ParaNode *node) {};

void Gen::visit(InvokeNode *node) {};

void Gen::visit(SelectNode *node) {};

