//
// Created by withparadox2 on 2016/8/25.
//

#include <iostream>
#include "Node.h"
#include "Utils.h"

using namespace std;


ClassNode::ClassNode(std::string name) : name(name), superClass(nullptr) {}

ClassNode::~ClassNode() {
    if (superClass != nullptr) {
        delete superClass;
    }
    releaseVec(funcNodes);
}

ModifierNode::ModifierNode(int modifier) : modifier(modifier) {}

TypeNode::TypeNode(int type, std::string &name) : type(type), name(name) {}

std::string TypeNode::buildPath(std::string separator) {
    if (name == "String") {
        return "java/lang/String";
    } else if (name == "int") {
        return "I";
    }
    return "";
}

ParaNode::ParaNode(TypeNode *typeNode, std::string &name) : typeNode(typeNode), name(name) {}

ParaNode::~ParaNode() {
    if (typeNode != nullptr) {
        delete typeNode;
    }
}

SelectNode::SelectNode(std::string name) : name(name), next(nullptr) {}

ConstantBase *SelectNode::genConstantRef(Pool &pool, ClassNode *rootNode) {
    if (name == "super") {
        string &superClass = rootNode->superClass->name;
        string funcName = "<init>";
        string funcType = "()V";
        return pool.genMethodref(superClass, funcName, funcType);
    } else if (name == "println") {
        return pool.genMethodref("java/io/PrintStream", name, "(Ljava/lang/String;)V");
    } else if (name == "out") {
        return pool.genFieldref("java/lang/System", "out", "Ljava/io/PrintStream;");
    }
    return nullptr;
}

SelectNode::~SelectNode() {
    if (next != nullptr) {
        delete next;
    }
}

StringLiteralNode::StringLiteralNode(std::string &val) : value(val) {}

InvokeNode::InvokeNode() : selectNode(nullptr), argNode(nullptr) {}

void InvokeNode::genConstant(Pool &pool, ClassNode *rootNode) {
    auto tempNode = selectNode;
    while (tempNode != nullptr) {
        string &name = tempNode->name;
        tempNode->genConstantRef(pool, rootNode);
        tempNode = tempNode->next;
    }

    if (argNode != nullptr) {
        pool.genString(argNode->value);
    }
}

InvokeNode::~InvokeNode() {
    if (selectNode != nullptr) {
        delete selectNode;
    }
    if (argNode != nullptr) {
        delete argNode;
    }
}

FuncNode::FuncNode() : returnTypeNode(nullptr) {}

std::string FuncNode::buildDescriptor() {
    std::string rType;
    if (returnTypeNode == nullptr || returnTypeNode->name == "void") {
        rType = "V";
    }
    std::string pType;
    for (auto iter = parameterNodes.begin(); iter != parameterNodes.end(); iter++) {
        auto pNode = *iter;
        if (pNode->typeNode->isArray) {
            pType += "[";
        }
        bool isObject = pNode->typeNode->type == TypeNode::TYPE_OBJECT;
        if (isObject) {
            pType += "L";
        }
        pType += pNode->typeNode->buildPath("/");
        if (isObject) {
            pType += ";";
        }
    }
    return "(" + pType + ")" + rType;
}

FuncNode::~FuncNode() {
    if (returnTypeNode != nullptr) {
        delete returnTypeNode;
    }
    releaseVec(modifierNodes);
    releaseVec(parameterNodes);
    releaseVec(statementNodes);
}
