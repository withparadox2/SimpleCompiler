//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SYMBOL_H
#define SIMPLECOMPILER_SYMBOL_H

#include <memory>
#include <vector>
#include "../comp/completer.h"
#include "types.h"
#include "../util/names.h"

class Name;
class Scope;

typedef std::shared_ptr<Scope> ScopePtr;

//TODO internal linkage, find out a way.
namespace Kind {
    const int NIL = 0;
    const int PKG = 1 << 0;
    /**type symbols (classes, interfaces and type variables).*/
    const int TYP = 1 << 1;
    /**variable symbols.*/
    const int VAR = 1 << 2;
    /**values (variables or non-variable expressions), includes VAR.*/
    const int VAL = (1 << 3) | VAR;
    /**methods*/
    const int MTH = (1 << 4);
    /**The error kind, which includes all other kinds.*/
    const int ERR = (1 << 5);
};

class Symbol : public std::enable_shared_from_this<Symbol> {
public:
    typedef std::shared_ptr<Symbol> Ptr;
    typedef std::vector<Ptr> List;
    typedef std::weak_ptr<Symbol> WeakPtr;
    long long flags;
    const Name& name;
    Symbol::Ptr owner;
    TypePtr type;
    int kind;
    //TODO ClassReader(static instance) is ok, how about a new Completer
    Completer* completer;

    Symbol(int kind, long flags, const Name& name, TypePtr type, Symbol::Ptr owner);
};

class TypeSymbol : public Symbol {
public:
    typedef std::shared_ptr<TypeSymbol> Ptr;
    typedef std::weak_ptr<TypeSymbol> WeakPtr;

    TypeSymbol(long flags, const Name& name, TypePtr type, Symbol::Ptr owner);
};

class ClassSymbol : public TypeSymbol {
public:
    typedef std::shared_ptr<ClassSymbol> Ptr;
    typedef std::weak_ptr<ClassSymbol> WeakPtr;

    /** a scope for all class members; variables, methods and inner classes
     *  type parameters are not part of this scope
     *
     *  But the method's symbol is actually entered into this. TODO check out why.
     */
    ScopePtr memberField;

    ScopePtr member();

    Name* fullName;

    void initOnShared();

    ClassSymbol(long flags, const Name& name, Symbol::Ptr owner);

    ClassSymbol(long flags, const Name& name, TypePtr type, Symbol::Ptr owner);
};

class VarSymbol : public Symbol {
public:
    typedef std::shared_ptr<VarSymbol> Ptr;

    VarSymbol(long flags, const Name& name, TypePtr type, Symbol::Ptr owner);
};

class MethodSymbol : public Symbol {
public:
    typedef std::shared_ptr<MethodSymbol> Ptr;

    MethodSymbol(long flags, const Name& name, TypePtr type, Symbol::Ptr owner);
};
class OperatorSymbol : public MethodSymbol {
public:
    typedef std::shared_ptr<OperatorSymbol> Ptr;

    int opcode;
    OperatorSymbol(const Name& name, TypePtr type, int opcode, Symbol::Ptr owner);
};

#endif //SIMPLECOMPILER_SYMBOL_H
