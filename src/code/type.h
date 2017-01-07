//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_TYPE_H
#define SIMPLECOMPILER_TYPE_H

#include <vector>

using std::vector;

class TypeSymbol;

class Type {
public:
    int tag;
    TypeSymbol* tsym;

    Type(int tag, TypeSymbol* tsym);
};

class ClassType : public Type {
public:
    Type* supertype_field;

    ClassType(TypeSymbol* tsym);
};

class MethodType : public Type {
public:
    vector<Type*>* argtypes;
    Type* restype;

    MethodType(vector<Type*>* argtypes,
               Type* restype,
               TypeSymbol* methodClass);
};

#endif //SIMPLECOMPILER_TYPE_H
