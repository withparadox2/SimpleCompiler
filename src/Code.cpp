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
        SelectNode *sNode = new SelectNode("java");
        rootNode->superClass = sNode;
        sNode->next = new SelectNode("lang");
        sNode->next->next = new SelectNode("Object");
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

        StatementNode *statementNode = new StatementNode;
        SelectNode *objNode = new SelectNode("super");
        statementNode->selectNode = objNode;
        funcNode->statementNodes.push_back(statementNode);
        funcNodes.push_back(funcNode);
    }

    pool.genClass(rootNode->name);
    pool.genClass(rootNode->superClass->buildPath("/"));

    for (auto iter = funcNodes.begin(); iter != funcNodes.end(); iter++) {
        pool.genUtf8((*iter)->name);
        pool.genUtf8((*iter)->buildDescriptor());
    }

    pool.buildConstantList();
    pool.writeStream(nullptr);
}

