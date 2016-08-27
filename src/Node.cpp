//
// Created by withparadox2 on 2016/8/25.
//

#include "Node.h"

ClassNode::ClassNode(Node *funcNode, std::string &name) : funcNode(funcNode), name(name) {}

ModifierNode::ModifierNode(int modifier) : modifier(modifier) {}

TypeNode::TypeNode(int type, std::string &name) : type(type), name(name) {}

ParaNode::ParaNode(Node *typeNode, std::string &name) : typeNode(typeNode), name(name) {}