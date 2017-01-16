//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SYMBOL_H
#define SIMPLECOMPILER_SYMBOL_H

#include <memory>
#include <vector>
#include "../comp/completer.h"

class Type;
class Name;

typedef std::shared_ptr<Type> TypePtr;

class ClassType;

class MethodType;

class Scope;
typedef std::shared_ptr<Scope> ScopePtr;


namespace Kind {
    extern int PKG;
/**type symbols (classes, interfaces and type variables).*/
    extern int TYP;
/**variable symbols.*/
    extern int VAR;
/**values (variables or non-variable expressions), includes VAR.*/
    extern int VAL;
/**methods*/
    extern int MTH;
/**The error kind, which includes all other kinds.*/
    extern int ERR;
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
//class OperatorSymbol : public MethodSymbol {
//
//};

#endif //SIMPLECOMPILER_SYMBOL_H
