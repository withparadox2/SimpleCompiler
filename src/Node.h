//
// Created by withparadox2 on 2016/8/25.
//

#ifndef SIMPLECOMPILER_NODE_H
#define SIMPLECOMPILER_NODE_H

#include <string>
#include <vector>
#include "Pool.h"

using std::vector;

class ClassNode;
class FuncNode;
class ModifierNode;
class TypeNode;
class ParaNode;
class InvokeNode;
class SelectNode;
class StringLiteralNode;

class Node {
};

class ClassNode : public Node {
public:
    vector<FuncNode *> funcNodes;
    std::string name;
    ClassNode *superClass;
    ClassNode(std::string name);
};

class FuncNode : public Node {
public:
    FuncNode();
    std::string name;
    vector<ModifierNode *> modifierNodes;
    TypeNode *returnTypeNode;
    vector<ParaNode *> parameterNodes;
    vector<InvokeNode *> statementNodes;
    std::string buildDescriptor();
};

class ModifierNode : public Node {
private:
public:
    int modifier;

    const static int STATIC = 0x0008;
    const static int PUBLIC = 0x0001;

    ModifierNode(int modifier);
};

class TypeNode : public Node {
private:
public:
    bool isArray;
    std::string name;
    int type;

    const static int TYPE_OBJECT = 1;
    const static int TYPE_PRIMITIVE = 2;

    TypeNode(int type, std::string &name);
    std::string buildPath(std::string separator);
};

class ParaNode : public Node {
public:
    TypeNode* typeNode;
    std::string name;
    ParaNode(TypeNode *typeNode, std::string &name);
};

class InvokeNode : public Node {
public:
    InvokeNode();
    SelectNode* selectNode;
    StringLiteralNode* argNode;
    void genConstant(Pool &pool, ClassNode *rootNode);
    std::string funcType();
};

class SelectNode : public Node {
public:
    SelectNode* next;
    std::string name;
    SelectNode(std::string name);
    std::string buildPath(std::string separator);

    ConstantBase *genConstantRef(Pool &pool, ClassNode *rootNode);
};

class StringLiteralNode : public Node {
public:
    std::string value;
    StringLiteralNode(std::string &val);
};


#endif //SIMPLECOMPILER_NODE_H
