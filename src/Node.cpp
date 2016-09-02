//
// Created by withparadox2 on 2016/8/25.
//

#include "Node.h"

ClassNode::ClassNode(std::string &name) : name(name), superClass(nullptr) {}

ModifierNode::ModifierNode(int modifier) : modifier(modifier) {}

TypeNode::TypeNode(int type, std::string &name) : type(type), name(name) {}

std::string TypeNode::buildPath(std::string separator) {
    if (name == "String") {
        return "java" + separator + "lang" + separator + name;
    } else if (name == "int") {
        return "I";
    }
    return "";
}

ParaNode::ParaNode(TypeNode *typeNode, std::string &name) : typeNode(typeNode), name(name) {}

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

ExpressionNode::ExpressionNode(std::string &val) : value(val) {}

StatementNode::StatementNode() : selectNode(nullptr), paraNode(nullptr) {}

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
