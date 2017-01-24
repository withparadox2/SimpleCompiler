//
// Created by withparadox2 on 2017/1/16.
//

#ifndef SIMPLECOMPILER_TYPES_H
#define SIMPLECOMPILER_TYPES_H

#include <memory>
#include <vector>

class Type;

typedef std::shared_ptr<Type> TypePtr;
typedef std::vector<TypePtr> TypeList;

class ClassType;
typedef std::shared_ptr<ClassType> ClassTypePtr;

class MethodType;
typedef std::shared_ptr<MethodType> MethodTypePtr;

#endif //SIMPLECOMPILER_TYPES_H
