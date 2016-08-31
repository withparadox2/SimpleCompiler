//
// Created by withparadox2 on 2016/8/31.
//

#include "Pool.h"

using namespace std;

ConstantBase *ConstantPolymer::isExist(string val, int tag) {
    if (map.find(val) == map.end()) {
        return nullptr;
    }
    auto vec = map[val];
    for (size_t i = vec.size() - 1; i >= 0; i--) {
        ConstantBase *p = vec.at(i);
        if (p->tag == tag) {
            return p;
        }
    }
    return nullptr;
}

void ConstantPolymer::put(string val, ConstantBase *item) {
    if (map.find(val) == map.end()) {
        map[val];
    }
    auto vec = map[val];
    vec.push_back(item);
}

void ConstantBase::bindFirstFunc(ConstantBase *p) {
    function<void(int)> fun = bind(&ConstantBase::onFirstPosSet, this);
    p->ref(fun);
}

void ConstantBase::bindSecondFunc(ConstantBase *p) {
    function<void(int)> fun = bind(&ConstantBase::onSecondPosSet, this);
    p->ref(fun);
}

void ConstantBase::onSecondPosSet(int pos) {
    this->refFirstPos = pos;
}

void ConstantBase::onFirstPosSet(int pos) {
    this->refSecondPos = pos;
}

void ConstantBase::ref(function<void(int)> ref) {
    if (index >= 0) {
        ref(index);
    }
    this->refers.push_back(ref);
}

void ConstantBase::setIndex(int index) {
    this->index = index;
    for (int i = refers.size() - 1; i >= 0; i--) {
        auto fun = refers[i];
        fun(index);
    }
}

ConstantBase::ConstantBase(int tag, string val) : tag(tag), index(-1), value(val) {}

ConstantBase::ConstantBase(int tag, ConstantBase *ref1) : tag(tag), index(-1) {
    bindFirstFunc(ref1);
}

ConstantBase::ConstantBase(int tag, ConstantBase *ref1, ConstantBase *ref2) : tag(tag), index(-1) {
    bindFirstFunc(ref1);
    bindSecondFunc(ref2);
}

ConstantBase *Pool::genUtf8(string val) {
    ConstantBase *item = polymer.isExist(val, CONSTANT_Utf8);
    if (item == nullptr) {
        item = new ConstantBase(CONSTANT_Utf8, val);
        polymer.put(val, item);
    }
    return item;
}

ConstantBase *Pool::genClass(string val) {
    ConstantBase *item = polymer.isExist(val, CONSTANT_Class);
    if (item == nullptr) {
        ConstantBase *ctUtf8 = genUtf8(val);
        item = new ConstantBase(CONSTANT_Class, ctUtf8);
        polymer.put(val, item);
    }
    return item;
}

ConstantBase *Pool::genString(string val) {
    ConstantBase *item = polymer.isExist(val, CONSTANT_String);
    if (item == nullptr) {
        ConstantBase *ctUtf8 = genUtf8(val);
        item = new ConstantBase(CONSTANT_String, ctUtf8);
        polymer.put(val, item);
    }
    return item;
}

ConstantBase *Pool::genNameAndType(string ref1, string ref2) {
    string val = ref1 + ref2;
    ConstantBase *item = polymer.isExist(val, CONSTANT_NameAndType);
    if (item == nullptr) {
        ConstantBase *ctUtf8Ref1 = genUtf8(ref1);
        ConstantBase *ctUtf8Ref2 = genUtf8(ref2);
        item = new ConstantBase(CONSTANT_NameAndType, ctUtf8Ref1, ctUtf8Ref2);
        polymer.put(val, item);
    }
    return item;
}

ConstantBase *Pool::genMethodref(string ref1, string ref2, string ref3) {
    string val = ref1 + ref2 + ref3;
    ConstantBase *item = polymer.isExist(val, CONSTANT_Methodref);
    if (item == nullptr) {
        ConstantBase *ctNameType = genNameAndType(ref2, ref3);
        ConstantBase *ctClass = genClass(ref1);
        item = new ConstantBase(CONSTANT_Methodref, ctClass, ctNameType);
        polymer.put(val, item);
    }
    return item;
}

ConstantBase *Pool::genFieldref(string ref1, string ref2, string ref3) {
    string val = ref1 + ref2 + ref3;
    ConstantBase *item = polymer.isExist(val, CONSTANT_Fieldref);
    if (item == nullptr) {
        ConstantBase *ctNameType = genNameAndType(ref2, ref3);
        ConstantBase *ctClass = genClass(ref1);
        item = new ConstantBase(CONSTANT_Fieldref, ctClass, ctNameType);
        polymer.put(val, item);
    }
    return item;
}
