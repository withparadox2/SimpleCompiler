//
// Created by withparadox2 on 2016/8/31.
//

#include <iostream>
#include "Code.h"
#include "Pool.h"

Code::Code(ClassNode *classNode, std::string path) : rootNode(classNode) {
    ofs.open(path, std::ofstream::binary | std::ofstream::trunc);
}

void Code::start() {
    if (ofs.is_open()) {
        writeHeader();
        writeConstantPool();
        writeClassInfo();
        writeMethods();
        writeAttribute();
        ofs.close();
    }
}

void Code::writeHeader() {
    w4B(0xcafebabe);
    w2B(0);
    w2B(51);
}

void Code::writeConstantPool() {
    prepare(pool);
    w2B(pool.ctVec.size() + 1);
    for (int i = 0, len = pool.ctVec.size(); i < len; i++) {
        ConstantBase *item = pool.ctVec[i];
        w1B(item->tag);
        if (item->refFirstPos >= 0) {
            w2B(item->refFirstPos + 1);
        }
        if (item->refSecondPos >= 0) {
            w2B(item->refSecondPos + 1);
        }
        if (item->tag == CONSTANT_Utf8) {
            w2B(item->value.size());
            ofs.write(item->value.c_str(), item->value.size());
        }
    }
}

void Code::writeClassInfo() {
    w2B(0x20);//acc_super
    int classIndex = pool.genClass(rootNode->name)->index + 1;
    w2B(classIndex);
    int superIndex = pool.genClass(rootNode->superClass->name)->index + 1;
    w2B(superIndex);
    w2B(0);//interface count
    w2B(0);//fields count
    w2B(rootNode->funcNodes.size());//methods count
}

void Code::writeMethods() {
    auto &funcNodes = rootNode->funcNodes;
    for (auto iter = funcNodes.rbegin(); iter != funcNodes.rend(); iter++) {
        FuncNode *node = *iter;
        w2B(buildModifierFlag(node->modifierNodes));
        w2B(pool.genUtf8(node->name)->index + 1);//name_index
        w2B(pool.genUtf8(node->buildDescriptor())->index + 1);//descriptor
        w2B(1);//attribute count
        w2B(pool.genUtf8("Code")->index + 1);

        if (node->name == "<init>") {
            w4B(17);
            w2B(1);
            w2B(1);
            w4B(5);//code length
            w1B(0x2a);//aload this
            w1B(0xb7);//invoke special

            int superFuncIndex = node->statementNodes[0]->selectNode->genConstantRef(pool, rootNode)->index + 1;
            w2B(superFuncIndex);
            w1B(0xb1);//return
        } else {
            SelectNode *selectNode = node->statementNodes[0]->selectNode;
            w4B(21);
            w2B(2);
            w2B(1);
            w4B(9);
            w1B(0xb2);
            w2B(selectNode->next->genConstantRef(pool, rootNode)->index + 1);
            w1B(0x12);
            w1B(pool.genString(node->statementNodes[0]->argNode->value)->index + 1);
            w1B(0xb6);
            w2B(selectNode->next->next->genConstantRef(pool, rootNode)->index + 1);
            w1B(0xb1);
        }
        w2B(0);//exception table length
        w2B(0);//attribute count
    }
}

void Code::writeFields() {

}

void Code::writeAttribute() {
    w2B(0);
}

void Code::prepare(Pool &pool) {
    if (rootNode->superClass == nullptr) {
        rootNode->superClass = new ClassNode("java/lang/Object");
    }

    auto &funcNodes = rootNode->funcNodes;
    bool hasDefineConstructor = false;
    for (auto iter = funcNodes.begin(); iter != funcNodes.end(); iter++) {
        if ((*iter)->name == rootNode->name) {
            hasDefineConstructor = true;
        }
    }
    if (!hasDefineConstructor) {
        FuncNode *funcNode = new FuncNode;
        funcNode->name = "<init>";

        InvokeNode *statementNode = new InvokeNode;
        SelectNode *objNode = new SelectNode("super");
        statementNode->selectNode = objNode;
        funcNode->statementNodes.push_back(statementNode);
        funcNodes.push_back(funcNode);
    }

    pool.genUtf8("Code");
    pool.genUtf8("LineNumberTable");
    pool.genUtf8("SourceFile");
    pool.genUtf8("HelloWorld.java");
    pool.genClass(rootNode->name);
    pool.genClass(rootNode->superClass->name);

    for (auto iter = funcNodes.begin(); iter != funcNodes.end(); iter++) {
        FuncNode *funcNode = *iter;
        pool.genUtf8(funcNode->name);
        pool.genUtf8(funcNode->buildDescriptor());

        InvokeNode *invokeNode = funcNode->statementNodes[0];
        invokeNode->genConstant(pool, rootNode);
    }

    pool.buildConstantList();
    pool.writeStream(nullptr);
}

template<class T>
void Code::wBytes(T integer, int length) {
    u1 *array = new u1[length];
    for (int i = 0; i < length; ++i) {
        array[length - 1 - i] = integer >> (8 * i);
    }
    ofs.write((const char *) array, length);
    delete[]array;
}

void Code::w1B(u1 bytes) {
    wBytes(bytes, 1);
}

void Code::w2B(u2 bytes) {
    wBytes(bytes, 2);
}

void Code::w4B(u4 bytes) {
    wBytes(bytes, 4);
}

u2 Code::buildModifierFlag(vector<ModifierNode *> &nodes) {
    u2 flag = 0;
    for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
        flag |= (*iter)->modifier;
    }
    return flag;
}

