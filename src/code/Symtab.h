//
// Created by withparadox2 on 2016/12/31.
//

#ifndef SIMPLECOMPILER_SYMTAB_H
#define SIMPLECOMPILER_SYMTAB_H

#include "type.h"
#include "../jvm/ClassReader.h"
#include <string>
#include "../util/names.h"
#include "Flags.h"
#include "TypeTags.h"

using std::string;

/**
 * predefined symbol, type
 */
class Symtab {
private:
    ClassReader& reader;
    Names& names;
    // For int, boolean types, the symbol is dangling, so we provided
    // a root for it.
    Symbol::List noRootSymbols;
    Symtab();

    Type::Ptr enterClass(const string& fullName);

    void initType(Type::Ptr& type, std::string name);

public:
    static Symtab& instance();
    Type::Ptr typeOfTag[TypeTags::TypeTagCount];

    ClassSymbol::Ptr arrayClass;
    ClassSymbol::Ptr methodClass;
    Symbol::Ptr noSymbol;

    Type::Ptr intType;
    Type::Ptr booleanType;
    Type::Ptr bolType;
    Type::Ptr voidType;
    Type::Ptr objectType;
    Type::Ptr classType;
    Type::Ptr stringType;
    Type::Ptr systemType;
    Type::Ptr printStreamType;
};


#endif //SIMPLECOMPILER_SYMTAB_H
