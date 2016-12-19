//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SYMBOL_H
#define SIMPLECOMPILER_SYMBOL_H

#include "../util/names.h"
#include "type.h"
#include "scope.h"

class Symbol {
public:
    int kind;
    long flags;
    Name* name;
    Symbol* owner;
    Type *type;
};

class TypeSymbol : public Symbol {

};

class ClassSymbol : public TypeSymbol {
public:
    Scope *memberField;
    Name *fullName;

};

class VarSymbol : public Symbol {

};

class MethodSymbol : public Symbol {

};

class OperatorSymbol : public MethodSymbol {

};

#endif //SIMPLECOMPILER_SYMBOL_H
