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
private:
    FuncNode *funcNode;
    std::string &name;
public:
    ClassNode(FuncNode *funcNode, std::string &name);
};

class FuncNode : public Node {
public:
    std::string name;
    vector<ModifierNode *> modifierNodes;
    TypeNode *returnTypeNode;
    vector<ParaNode *> parameterNodes;
    vector<StatementNode *> statementNodes;
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
    int type;
    std::string &name;
public:
    bool isArray;

    const static int TYPE_OBJECT = 1;
    const static int TYPE_PRIMITIVE = 2;

    TypeNode(int type, std::string &name);
};

class ParaNode : public Node {
private:
    TypeNode* typeNode;
    std::string &name;
public:
    ParaNode(TypeNode *typeNode, std::string &name);
};

class StatementNode : public Node {
public:
    SelectNode* selectNode;
    ExpressionNode* paraNode;
};

class SelectNode : public Node {
public:
    SelectNode* next;
    std::string &name;
    SelectNode(std::string &name);
};

class ExpressionNode : public Node {
public:
    std::string value;
    ExpressionNode(std::string &val);
};


#endif //SIMPLECOMPILER_NODE_H
