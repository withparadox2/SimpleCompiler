//
// Created by withparadox2 on 2016/8/31.
//

#ifndef SIMPLECOMPILER_POOL_H
#define SIMPLECOMPILER_POOL_H

#include <functional>
#include <vector>
#include <map>
#include <fstream>


const int CONSTANT_Class = 7;
const int CONSTANT_Fieldref = 9;
const int CONSTANT_Methodref = 10;
const int CONSTANT_InterfaceMethodref = 11;
const int CONSTANT_String = 8;
const int CONSTANT_Integer = 3;
const int CONSTANT_Float = 4;
const int CONSTANT_Long = 5;
const int CONSTANT_Double = 6;
const int CONSTANT_NameAndType = 12;
const int CONSTANT_Utf8 = 1;
const int CONSTANT_MethodHandle = 15;
const int CONSTANT_MethodType = 16;
const int CONSTANT_InvokeDynamic = 18;


class Pool;

class ConstantBase;

class ConstantPolymer;

using namespace std;

class ConstantPolymer {
    map<string, ConstantBase *> ctMap;
public:
    ConstantBase *isExist(string val, int tag);

    void put(string val, ConstantBase *item);

    void buildList(vector<ConstantBase *> &vec);

    ~ConstantPolymer();
};

class ConstantBase {
private:
    vector<function<void(int)>> refers;

public:
    string value;
    int index;//在常量池中的位置
    int refFirstPos;
    int refSecondPos;

    int tag;

    ConstantBase(int tag, string val);

    ConstantBase(int tag, ConstantBase *ref1);

    ConstantBase(int tag, ConstantBase *ref1, ConstantBase *ref2);

    void init();

    void setIndex(int index);

    void ref(function<void(int)> ref);

    void onFirstPosSet(int pos);

    void onSecondPosSet(int pos);

    void bindFirstFunc(ConstantBase *p);

    void bindSecondFunc(ConstantBase *p);
};

class Pool {
public:
    Pool();

    vector<ConstantBase *> ctVec;

    ConstantPolymer polymer;

    void buildConstantList();

    void writeStream(ofstream *stream);

    ConstantBase *genUtf8(string val);

    ConstantBase *genClass(string val);

    ConstantBase *genString(string val);

    ConstantBase *genNameAndType(string ref1, string ref2);

    ConstantBase *genMethodref(string ref1, string ref2, string ref3);

    ConstantBase *genFieldref(string ref1, string ref2, string ref3);
};


#endif //SIMPLECOMPILER_POOL_H
