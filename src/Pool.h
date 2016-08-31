//
// Created by ruancongyong on 2016/8/31.
//

#ifndef SIMPLECOMPILER_POOL_H
#define SIMPLECOMPILER_POOL_H
#include <functional>
#include <vector>

class Pool;

class ConstantClass;
class ConstantUtf8;
class ConstantMethodref;
class ConstantFieldref;
class ConstantString;
class ConstantNameAndType;

class ConstantPolymer {
    std::string value;

    ConstantClass *ctClass;
    ConstantUtf8 *ctUtf8;
    ConstantMethodref *ctMethodref;
    ConstantFieldref *ctFieldref;
    ConstantString *ctString;
    ConstantNameAndType *ctNameAndType;
};

class ConstantBase {
private:
    std::vector<std::function<void(int)>> refers;
    int index;//在常量池中的位置
    int refFirstPos;
    int refSecondPos;
    int tag;
public:
    ConstantBase(int tag);
    void setIndex();
    void ref(std::function<void(int)> ref);
    void onFirstPosSet(int pos);
    void onSecondPosSet(int pos);
};

class ConstantClass : ConstantBase {
public:
    ConstantClass(int tag);
};

class Pool {
};


#endif //SIMPLECOMPILER_POOL_H
