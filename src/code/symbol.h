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
#include "symbols.h"

class Name;
class Scope;

typedef std::shared_ptr<Scope> ScopePtr;

//TODO internal linkage, find out a way.
namespace Kind {
    enum {
        NIL = 0,
        PKG = 1 << 0,
        /**type symbols (classes, interfaces and type variables).*/
        TYP = 1 << 1,
        /**variable symbols.*/
        VAR = 1 << 2,
        /**values (variables or non-variable expressions), includes VAR.*/
        VAL = (1 << 3) | VAR,
        /**methods*/
        MTH = (1 << 4),
        /**The error kind, which includes all other kinds.*/
        ERR = (1 << 5)
    };
};

class Symbol : public std::enable_shared_from_this<Symbol> {
public:
    long long flags;
    const Name& name;
    SymbolPtr owner;
    TypePtr type;
    int kind;
    //TODO ClassReader(static instance) is ok, how about a new Completer
    Completer* completer;

    Symbol(int kind, long flags, const Name& name, TypePtr type, SymbolPtr owner);
    virtual ScopePtr member();
};

class TypeSymbol : public Symbol {
public:
    TypeSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner);
};

class ClassSymbol : public TypeSymbol {
public:
    /** a scope for all class members; variables, methods and inner classes
     *  type parameters are not part of this scope
     *
     *  But the method's symbol is actually entered into this. TODO check out why.
     */
    ScopePtr memberField;

    ScopePtr member();

    Name* fullName;

    SymbolPtr initOnShared();

    ClassSymbol(long flags, const Name& name, SymbolPtr owner);

    ClassSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner);
};

class VarSymbol : public Symbol {
public:
    VarSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner);
};

class MethodSymbol : public Symbol {
public:
    MethodSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner);
};
class OperatorSymbol : public MethodSymbol {
public:
    int opcode;
    OperatorSymbol(const Name& name, TypePtr type, int opcode, SymbolPtr owner);
};

#endif //SIMPLECOMPILER_SYMBOL_H
