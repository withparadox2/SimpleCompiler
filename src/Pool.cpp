//
// Created by withparadox2 on 2016/8/31.
//

#include <iostream>
#include "Pool.h"

using namespace std;


ConstantBase *ConstantPolymer::isExist(string val, int tag) {
    string key = val + to_string(tag);
    if (ctMap.find(key) == ctMap.end()) {
        return nullptr;
    }
    auto p = ctMap[key];
    if (p->tag == tag) {
        return p;
    }
    return nullptr;
}

void ConstantPolymer::put(string val, ConstantBase *item) {
    string key = val + to_string(item->tag);
    if (ctMap.find(key) == ctMap.end()) {
        ctMap.insert(make_pair(key, item));
    }
}

ConstantPolymer::~ConstantPolymer() {
    for (auto it = ctMap.begin(); it != ctMap.end(); ++it) {
        delete it->second;
    }
}

void ConstantPolymer::buildList(vector<ConstantBase *> &ctVec) {
    for (auto it = ctMap.begin(); it != ctMap.end(); ++it) {
        ConstantBase *item = it->second;
        item->setIndex(ctVec.size());
        ctVec.push_back(item);
    }
}

void ConstantBase::bindFirstFunc(ConstantBase *p) {
    function<void(int)> fun = bind(&ConstantBase::onFirstPosSet, this, placeholders::_1);
    p->ref(fun);
}

void ConstantBase::bindSecondFunc(ConstantBase *p) {
    function<void(int)> fun = bind(&ConstantBase::onSecondPosSet, this, placeholders::_1);
    p->ref(fun);
}

void ConstantBase::onSecondPosSet(int pos) {
    this->refSecondPos = pos;
}

void ConstantBase::onFirstPosSet(int pos) {
    this->refFirstPos = pos;
}

void ConstantBase::ref(function<void(int)> ref) {
    if (index >= 0) {
        ref(index);
    }
    this->refers.push_back(ref);
}

void ConstantBase::setIndex(int index) {
    this->index = index;
    for (auto iter = refers.begin(); iter != refers.end(); ++iter) {
        auto fun = *iter;
        fun(index);
    }
}

ConstantBase::ConstantBase(int tag, string val) : tag(tag), value(val) {
    init();
}

ConstantBase::ConstantBase(int tag, ConstantBase *ref1) : tag(tag) {
    init();
    bindFirstFunc(ref1);
}

ConstantBase::ConstantBase(int tag, ConstantBase *ref1, ConstantBase *ref2) : tag(tag) {
    init();
    bindFirstFunc(ref1);
    bindSecondFunc(ref2);
}

void ConstantBase::init() {
    this->index = -1;
    this->refFirstPos = -1;
    this->refSecondPos = -1;
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
        item->value = val;
        polymer.put(val, item);
    }
    return item;
}

ConstantBase *Pool::genString(string val) {
    ConstantBase *item = polymer.isExist(val, CONSTANT_String);
    if (item == nullptr) {
        ConstantBase *ctUtf8 = genUtf8(val);
        item = new ConstantBase(CONSTANT_String, ctUtf8);
        item->value = val;
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
        item->value = val;
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
        item->value = val;
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
        item->value = val;
        polymer.put(val, item);
    }
    return item;
}

Pool::Pool() {
//    genClass("HelloWorld");
//    genNameAndType("<init>", "()V");
//    genMethodref("java/lang/Object", "<init>", "()V");
//    buildConstantList();
//    writeStream(nullptr);
}

void Pool::buildConstantList() {
    polymer.buildList(ctVec);
}

void Pool::writeStream(ofstream *stream) {
    for (auto iter = ctVec.begin(); iter != ctVec.end(); ++iter) {
        auto item = *iter;
        cout << "tag = " << item->tag << "  value = " << item->value << endl;
        if (item->refFirstPos >= 0) {
            cout << "refFirstPos = " << item->refFirstPos << endl;
        }
        if (item->refSecondPos >= 0) {
            cout << "refSecondPos = " << item->refSecondPos << endl;
        }
        cout << "============================" << endl;
    }
}
