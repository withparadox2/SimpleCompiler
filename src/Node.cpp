//
// Created by withparadox2 on 2016/8/25.
//

#include "Node.h"

ClassNode::ClassNode(FuncNode *funcNode, std::string &name) : funcNode(funcNode), name(name) {}

ModifierNode::ModifierNode(int modifier) : modifier(modifier) {}

TypeNode::TypeNode(int type, std::string &name) : type(type), name(name) {}

ParaNode::ParaNode(TypeNode *typeNode, std::string &name) : typeNode(typeNode), name(name) {}

SelectNode::SelectNode(std::string &name) : name(name) {}

ExpressionNode::ExpressionNode(std::string &val) : value(val) {}