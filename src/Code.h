//
// Created by withparadox2 on 2016/8/31.
//

#ifndef SIMPLECOMPILER_CODE_H
#define SIMPLECOMPILER_CODE_H

#include "Node.h"

class Code {
private:
    ClassNode* rootNode;
    std::string classPath;

public:
    Code(ClassNode* classNode, std::string path);
    void start();
    void writeHeader();
    void writeConstantPool();
    void writeClassInfo();
    void writeMethods();
    void writeFields();
    void writeAttribute();
};


#endif //SIMPLECOMPILER_CODE_H
