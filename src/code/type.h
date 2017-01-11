//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_TYPE_H
#define SIMPLECOMPILER_TYPE_H

#include <vector>
#include <memory>
#include "./symbol.h"

using std::vector;

class Type {
public:
    typedef std::shared_ptr<Type> Ptr;
    typedef vector<Ptr> List;
    int tag;
    TypeSymbol::WeakPtr tsym;

    Type(int tag, TypeSymbol::Ptr tsym);
};

class ClassType : public Type {
public:
    typedef std::shared_ptr<ClassType> Ptr;

    Type::Ptr supertype_field;

    ClassType(TypeSymbol::Ptr tsym);
};

class MethodType : public Type {
public:
    typedef std::shared_ptr<MethodType> Ptr;

    vector<Type::Ptr> argtypes;
    Type::Ptr restype;

    MethodType(vector<Type::Ptr> argtypes,
               Type::Ptr restype,
               TypeSymbol::Ptr methodClass);
};

class ArrayType : public Type {
public:
    Type::Ptr elemtype;
    ArrayType(Type::Ptr elemtype, TypeSymbol::Ptr arrayClass);
};

#endif //SIMPLECOMPILER_TYPE_H
