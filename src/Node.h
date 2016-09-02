//
// Created by withparadox2 on 2016/8/25.
//

#ifndef SIMPLECOMPILER_NODE_H
#define SIMPLECOMPILER_NODE_H

#include <string>
#include <vector>

using std::vector;

class ClassNode;
class FuncNode;
class ModifierNode;
class TypeNode;
class ParaNode;
class StatementNode;
class SelectNode;
class ExpressionNode;

class Node {
};

class ClassNode : public Node {
public:
    vector<FuncNode *> funcNodes;
    std::string name;
    SelectNode *superClass;
    ClassNode(std::string &name);
};

class FuncNode : public Node {
public:
    FuncNode();
    std::string name;
    vector<ModifierNode *> modifierNodes;
    TypeNode *returnTypeNode;
    vector<ParaNode *> parameterNodes;
    vector<StatementNode *> statementNodes;
    std::string buildDescriptor();
};

class ModifierNode : public Node {
private:
    int modifier;
public:
    const static int STATIC = 1;
    const static int PUBLIC = 2;

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

class StatementNode : public Node {
public:
    StatementNode();
    SelectNode* selectNode;
    ExpressionNode* paraNode;
};

class SelectNode : public Node {
public:
    SelectNode* next;
    std::string name;
    SelectNode(std::string name);
    std::string buildPath(std::string separator);
};

class ExpressionNode : public Node {
public:
    std::string value;
    ExpressionNode(std::string &val);
};


#endif //SIMPLECOMPILER_NODE_H
