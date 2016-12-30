//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SYMBOL_H
#define SIMPLECOMPILER_SYMBOL_H

#include "../util/names.h"
#include "type.h"
#include "scope.h"
enum class Kind {
    PKG = 1 << 0,
    /**type symbols (classes, interfaces and type variables).*/
    TYP = 1 << 1,
    /**variable symbols.*/
    VAR = 1 << 2,
    /**values (variables or non-variable expressions), includes VAR.*/
    VAL = (1 << 3) | static_cast<int>(VAR),
    /**methods*/
    MTH = (1 << 4),
    /**The error kind, which includes all other kinds.*/
    ERR = (1 << 5)
};

class Symbol {
public:
    int kind;
    long flags;
    const Name& name;
    Symbol* owner;
    Type *type;
    Symbol(int kind, long flags, const Name &name, Type *type, Symbol *owner);
};

class TypeSymbol : public Symbol {
public:
    TypeSymbol(long flags, const Name &name, Type *type, Symbol *owner);
};

class ClassSymbol : public TypeSymbol {
public:
/**
 * memberFiled is used to store class member, since nested class is not allowed, so
 * just ignore it.
 */
// Scope *memberField;
    Name *fullName;
    ClassSymbol(long flags, const Name &name, Symbol *owner);
    ClassSymbol(long flags, const Name &name, Type *type, Symbol *owner);
};



//class VarSymbol : public Symbol {
//
//};
//
//class MethodSymbol : public Symbol {
//
//};

//class OperatorSymbol : public MethodSymbol {
//
//};

#endif //SIMPLECOMPILER_SYMBOL_H
