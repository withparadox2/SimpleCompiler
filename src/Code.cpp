//
// Created by withparadox2 on 2016/8/31.
//

#include <iostream>
#include "Code.h"
#include "Pool.h"

Code::Code(ClassNode *classNode, std::string path) : rootNode(classNode), classPath(path) {}

void Code::start() {
    Pool pool;
    prepare(pool);
}

void Code::writeHeader() {

}

void Code::writeConstantPool() {

}

void Code::writeClassInfo() {

}

void Code::writeMethods() {

}

void Code::writeFields() {

}

void Code::writeAttribute() {

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

