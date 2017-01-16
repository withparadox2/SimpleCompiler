//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_TYPE_H
#define SIMPLECOMPILER_TYPE_H

#include <vector>
#include <memory>
#include "symbols.h"

using std::vector;

class Type {
public:
    typedef std::shared_ptr<Type> Ptr;
    typedef vector<Ptr> List;
    int tag;
    TypeSymbolWeakPtr tsym;

    Type(int tag, TypeSymbolPtr tsym);
};

class ClassType : public Type {
public:
    typedef std::shared_ptr<ClassType> Ptr;

    Type::Ptr supertype_field;

    ClassType(TypeSymbolPtr tsym);
};

class MethodType : public Type {
public:
    typedef std::shared_ptr<MethodType> Ptr;

    vector<Type::Ptr> argtypes;
    Type::Ptr restype;

    MethodType(vector<Type::Ptr> argtypes,
               Type::Ptr restype,
               TypeSymbolPtr methodClass);
};

class ArrayType : public Type {
public:
    Type::Ptr elemtype;
    ArrayType(Type::Ptr elemtype, TypeSymbolPtr arrayClass);
};

#endif //SIMPLECOMPILER_TYPE_H
