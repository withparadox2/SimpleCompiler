//
// Created by withparadox2 on 2016/8/25.
//

#ifndef SIMPLECOMPILER_NODE_H
#define SIMPLECOMPILER_NODE_H

#include <string>
#include <vector>

using std::vector;

class Node {
};

class ClassNode : public Node {
private:
    Node *funcNode;
    std::string &name;
public:
    ClassNode(Node *funcNode, std::string &name);
};

class FuncNode : public Node {
public:
    std::string name;
    vector<Node *> modifierNodes;
    Node *returnTypeNode;
    vector<Node *> parameterNodes;
    vector<Node *> statementNodes;
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
    Node* typeNode;
    std::string &name;
public:
    ParaNode(Node *typeNode, std::string &name);
};

class StatementNode : public Node {
public:
    Node* selectNode;
    Node* paraNode;
};

class SelectNode : public Node {
public:
    SelectNode* next;
    std::string &name;
    SelectNode(std::string &name);
};

class StrNode : public Node {
public:
    std::string value;
    StrNode(std::string &val);
};


#endif //SIMPLECOMPILER_NODE_H
