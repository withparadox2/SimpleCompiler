//
// Created by withparadox2 on 2016/8/31.
//

#ifndef SIMPLECOMPILER_CODE_H
#define SIMPLECOMPILER_CODE_H

#include "Node.h"
#include "Pool.h"

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;
typedef int8_t s1;
typedef int16_t s2;
typedef int32_t s4;
typedef int64_t s8;

class Code {
private:
    ClassNode *rootNode;
    Pool pool;

    void prepare(Pool &pool);

    ofstream ofs;

    u1 arrBuffer[4];
public:
    Code(ClassNode *classNode, std::string path);

    void start();

    void writeHeader();

    void writeConstantPool();

    void writeClassInfo();

    void writeMethods();

    void writeFields();

    void writeAttribute();

    template<class T>
    void wBytes(T bytes, int length);

    void w1B(u1 bytes);

    void w2B(u2 bytes);

    void w4B(u4 bytes);

    u2 buildModifierFlag(vector<ModifierNode *> &nodes);

    ClassNode *getRootNode();
};


#endif //SIMPLECOMPILER_CODE_H
