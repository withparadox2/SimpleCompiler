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
    typedef std::shared_ptr<Type> Ptr;
    typedef vector<Ptr> List;
    int tag;
    TypeSymbolWeakPtr tsym;

    Type(int tag, TypeSymbolPtr tsym);
    virtual TypePtr getReturnType();
    virtual List getParameterTypes();
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
    TypePtr getReturnType();
    List getParameterTypes();
};

class ArrayType : public Type {
public:
    Type::Ptr elemtype;
    ArrayType(Type::Ptr elemtype, TypeSymbolPtr arrayClass);
};

#endif //SIMPLECOMPILER_TYPE_H
