//
// Created by withparadox2 on 2016/8/25.
//

#include <iostream>
#include "Node.h"

using namespace std;

ClassNode::ClassNode(std::string name) : name(name), superClass(nullptr) {}

void ClassNode::accept(Visitor &visitor) {
    visitor.visit(this);
}

ModifierNode::ModifierNode(int modifier) : modifier(modifier) {}

void ModifierNode::accept(Visitor &visitor) {
    visitor.visit(this);
}

TypeNode::TypeNode(int type, std::string &name) : type(type), name(name) {}

std::string TypeNode::buildPath(std::string separator) {
    if (name == "String") {
        return "java/lang/String";
    } else if (name == "int") {
        return "I";
    }
    return "";
}

void TypeNode::accept(Visitor &visitor) {
    visitor.visit(this);
}

ParaNode::ParaNode(TypeNode *typeNode, std::string &name) : typeNode(typeNode), name(name) {}

void ParaNode::accept(Visitor &visitor) {
    visitor.visit(this);
}

SelectNode::SelectNode(std::string name) : name(name), next(nullptr) {}

std::string SelectNode::buildPath(std::string separator) {
    std::string result = name;
    SelectNode *p = next;
    while (p != nullptr) {
        result += separator + p->name;
        p = p->next;
    }
    return result;
}

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

void SelectNode::accept(Visitor &visitor) {
    visitor.visit(this);
}

StringLiteralNode::StringLiteralNode(std::string &val) : value(val) {}

void StringLiteralNode::accept(Visitor &visitor) {
    visitor.visit(this);
}

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

std::string InvokeNode::funcType() {
    return std::string();
}

void InvokeNode::accept(Visitor &visitor) {
    visitor.visit(this);
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

void FuncNode::accept(Visitor &visitor) {
    visitor.visit(this);
}
