//
// Created by withparadox2 on 2016/12/31.
//

#ifndef SIMPLECOMPILER_SYMTAB_H
#define SIMPLECOMPILER_SYMTAB_H

#include "types.h"
#include "symbols.h"
#include <string>
#include "Flags.h"
#include "TypeTags.h"

using std::string;

class ClassReader;
class Names;

/**
 * predefined symbol, type
 */
class Symtab {
private:
    ClassReader& reader;
    Names& names;
    // Symbol of primitive type (int, boolean, void, null) is dangling, so we provided
    // a root for it.
    SymbolList noRootSymbols;
    Symtab();

    TypePtr enterClass(const string& fullName);

    void initType(TypePtr& type, std::string name);

public:
    static Symtab& instance();
    TypePtr typeOfTag[TypeTags::TypeTagCount];

    ClassSymbolPtr arrayClass;
    ClassSymbolPtr methodClass;
    SymbolPtr noSymbol;

    TypePtr intType;
    TypePtr booleanType;
    TypePtr botType;
    TypePtr voidType;
    TypePtr objectType;
    TypePtr unknownType;
    TypePtr noType;
    TypePtr classType;
    TypePtr stringType;
    TypePtr systemType;
    TypePtr printStreamType;
};


#endif //SIMPLECOMPILER_SYMTAB_H
