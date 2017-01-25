//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_TYPE_H
#define SIMPLECOMPILER_TYPE_H

#include <vector>
#include <memory>
#include "symbols.h"
#include "symbol.h"

using std::vector;

class Type {
public:
    int tag;
    TypeSymbolWeakPtr tsym;

    Type(int tag, TypeSymbolPtr tsym);
    virtual TypePtr getReturnType();
    virtual TypeList getParameterTypes();
    virtual ~Type();
};

class ClassType : public Type {
public:
    TypePtr supertype_field;

    ClassType(TypeSymbolPtr tsym);
};

class MethodType : public Type {
public:
    vector<TypePtr> argtypes;
    TypePtr restype;

    MethodType(vector<TypePtr> argtypes,
               TypePtr restype,
               TypeSymbolPtr methodClass);
    TypePtr getReturnType();
    TypeList getParameterTypes();
};

class ArrayType : public Type {
public:
    TypePtr elemtype;
    ArrayType(TypePtr elemtype, TypeSymbolPtr arrayClass);
};

#endif //SIMPLECOMPILER_TYPE_H
