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

class Node;

class Visitor {
public:
    virtual void visit(ClassNode *node) = 0;

    virtual void visit(FuncNode *node) = 0;

    virtual void visit(ModifierNode *node) = 0;

    virtual void visit(TypeNode *node) = 0;

    virtual void visit(ParaNode *node) = 0;

    virtual void visit(InvokeNode *node) = 0;

    virtual void visit(SelectNode *node) = 0;

    virtual void visit(StringLiteralNode *node) = 0;

};


class Node {
public:
    virtual void accept(Visitor &visitor) = 0;
};

class ClassNode : public Node {
public:
    vector<FuncNode *> funcNodes;
    std::string name;
    ClassNode *superClass;

    ClassNode(std::string name);
    void accept(Visitor &visitor) override;
    ~ClassNode();
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
    void accept(Visitor &visitor) override;
    ~FuncNode();
};

class ModifierNode : public Node {
private:
public:
    int modifier;

    const static int STATIC = 0x0008;
    const static int PUBLIC = 0x0001;

    ModifierNode(int modifier);

    void accept(Visitor &visitor) override;
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

    void accept(Visitor &visitor) override;

};

class ParaNode : public Node {
public:
    TypeNode *typeNode;
    std::string name;

    ParaNode(TypeNode *typeNode, std::string &name);
    void accept(Visitor &visitor) override;
    ~ParaNode();
};

class InvokeNode : public Node {
public:
    InvokeNode();

    SelectNode *selectNode;
    StringLiteralNode *argNode;

    void genConstant(Pool &pool, ClassNode *rootNode);
    std::string funcType();

    void accept(Visitor &visitor) override;

    ~InvokeNode();
};

class SelectNode : public Node {
public:
    SelectNode *next;
    std::string name;

    SelectNode(std::string name);

    std::string buildPath(std::string separator);

    ConstantBase *genConstantRef(Pool &pool, ClassNode *rootNode);

    void accept(Visitor &visitor) override;

    ~SelectNode();
};

class StringLiteralNode : public Node {
public:
    std::string value;

    StringLiteralNode(std::string &val);

    void accept(Visitor &visitor) override;
};




#endif //SIMPLECOMPILER_NODE_H
