//
// Created by withparadox2 on 2016/12/31.
//

#ifndef SIMPLECOMPILER_SYMTAB_H
#define SIMPLECOMPILER_SYMTAB_H

#include <string>
#include "types.h"
#include "symbols.h"
#include "Flags.h"
#include "TypeTags.h"
#include "symbol.h"
#include "util/context.h"

using std::string;

class ClassReader;
class Names;

/**
 * predefined symbol, type
 */
class Symtab : public EnableMapHelper {
private:
    ClassReader* reader;
    Names* names;
    // Symbol of primitive type (int, boolean, void, null) is dangling, so we provide
    // a root for it.
    Symtab();

    TypePtr enterClass(const string& fullName);

    void initType(TypePtr& type, std::string name);

    void enterUnop(std::string name, TypePtr arg, TypePtr res, int opcode);

    void enterBinop(std::string name, TypePtr left, TypePtr right, TypePtr res, int opcode);

    void setUpArrayType(ClassSymbolPtr sym);

public:
    ClassSymbolPtr predefClass;
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
    TypePtr anyType;
    TypePtr errorType;
    TypePtr classType;
    TypePtr stringType;
    TypePtr systemType;
    TypePtr printStreamType;
    TypePtr stringBuilderType;

    VarSymbolPtr lengthVar;
};


#endif //SIMPLECOMPILER_SYMTAB_H
