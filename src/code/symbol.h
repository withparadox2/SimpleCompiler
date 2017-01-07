//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SYMBOL_H
#define SIMPLECOMPILER_SYMBOL_H

#include "../util/names.h"
#include <memory>

class Type;

class ClassType;

class MethodType;

class Scope;

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
private:
    Kind kindinternal;
public:
    typedef std::shared_ptr<Symbol> Ptr;
    typedef std::weak_ptr<Symbol> WeakPtr;
    long long flags;
    const Name& name;
    Symbol::Ptr owner;
    Type* type;

    Symbol(Kind kind, long flags, const Name& name, Type* type, Symbol::Ptr owner);

    int kind();
};

class TypeSymbol : public Symbol {
public:
    typedef std::shared_ptr<TypeSymbol> Ptr;
    typedef std::weak_ptr<TypeSymbol> WeakPtr;

    TypeSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner);
};

class ClassSymbol : public TypeSymbol {
public:
    typedef std::shared_ptr<ClassSymbol> Ptr;
    typedef std::weak_ptr<ClassSymbol> WeakPtr;

    /** a scope for all class members; variables, methods and inner classes
     *  type parameters are not part of this scope
     *
     *  But method's symbol is actually entered into this. TODO check out why.
     */
    std::shared_ptr<Scope> memberField;
    Name* fullName;

    ClassSymbol(long flags, const Name& name, Symbol::Ptr owner);

    ClassSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner);
};

class VarSymbol : public Symbol {
public:
    typedef std::shared_ptr<VarSymbol> Ptr;

    VarSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner);
};

class MethodSymbol : public Symbol {
public:
    typedef std::shared_ptr<MethodSymbol> Ptr;

    MethodSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner);
};

//class OperatorSymbol : public MethodSymbol {
//
//};

#endif //SIMPLECOMPILER_SYMBOL_H
