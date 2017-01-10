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

using std::string;

/**
 * predefined symbol, type
 */
class Symtab {
private:
    ClassReader& reader;
    Names& names;

    Symtab();

    Type::Ptr enterClass(const string& fullName);

public:
    static Symtab& instance();

    ClassSymbol::Ptr arrayClass;
    Symbol::Ptr noSymbol;

    Type::Ptr voidType;
    Type::Ptr objectType;
    Type::Ptr classType;
    Type::Ptr stringType;
    Type::Ptr systemType;
    Type::Ptr printStreamType;
};


#endif //SIMPLECOMPILER_SYMTAB_H
