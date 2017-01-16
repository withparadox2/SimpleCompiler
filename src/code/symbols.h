//
// Created by withparadox2 on 2017/1/16.
//

#ifndef SIMPLECOMPILER_SYMBOLS_H
#define SIMPLECOMPILER_SYMBOLS_H

#include <memory>
#include <vector>

class Symbol;
typedef std::shared_ptr<Symbol> SymbolPtr;
typedef std::vector<SymbolPtr> SymbolList;
typedef std::weak_ptr<Symbol> SymbolWeakPtr;

class ClassSymbol;
typedef std::shared_ptr<ClassSymbol> ClassSymbolPtr;
typedef std::vector<ClassSymbolPtr> ClassSymbolList;
typedef std::weak_ptr<ClassSymbol> ClassSymbolWeakPtr;

class TypeSymbol;
typedef std::shared_ptr<TypeSymbol> TypeSymbolPtr;
typedef std::vector<TypeSymbolPtr> TypeSymbolList;
typedef std::weak_ptr<TypeSymbol> TypeSymbolWeakPtr;

class VarSymbol;
typedef std::shared_ptr<VarSymbol> VarSymbolPtr;

class MethodSymbol;
typedef std::shared_ptr<MethodSymbol> MethodSymbolPtr;


#endif //SIMPLECOMPILER_SYMBOLS_H
