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
private:
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
    const static int TYPE_OBJECT = 1;
    const static int TYPE_PRIMITIVE =2;

    TypeNode(int type, std::string &name);
};



#endif //SIMPLECOMPILER_NODE_H
