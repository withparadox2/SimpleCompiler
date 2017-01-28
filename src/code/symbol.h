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
    // Refer to single instance of ClassReader(static instance),
    // do not bother to clear it.
    Completer* completer;

    Symbol(int kind, long flags, const Name& name, TypePtr type, SymbolPtr owner);
    virtual ScopePtr member();
    virtual ~Symbol();
};

class TypeSymbol : public Symbol {
public:
    TypeSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner);
};

class ClassSymbol : public TypeSymbol {
public:
    /** A scope for all class members, including fields,
     *  methods and inner classes. Type parameters are
     *  not part of this scope.
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

class Code;
class MethodSymbol : public Symbol {
public:
    std::shared_ptr<Code> code;
    MethodSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner);
    bool isConstructor();
};
class OperatorSymbol : public MethodSymbol {
public:
    int opcode;
    OperatorSymbol(const Name& name, TypePtr type, int opcode, SymbolPtr owner);
};

#endif //SIMPLECOMPILER_SYMBOL_H
