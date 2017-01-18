//
// Created by withparadox2 on 2016/12/31.
//

#include "Symtab.h"
#include "type.h"
#include "../jvm/ClassReader.h"
#include "../util/names.h"
#include "symbol.h"

Symtab& Symtab::instance() {
    static Symtab inst;
    return inst;
}

Symtab::Symtab() : reader(ClassReader::instance()), names(Names::instance()), typeOfTag{Type::Ptr()} {
    intType = Type::Ptr(new Type(TypeTags::INT, nullptr));
    booleanType = Type::Ptr(new Type(TypeTags::BOOLEAN, nullptr));
    botType = Type::Ptr(new Type(TypeTags::BOT, nullptr));
    voidType = Type::Ptr(new Type(TypeTags::VOID, nullptr));
    unknownType = Type::Ptr(new Type(TypeTags::UNKNOWN, nullptr));
    noType = Type::Ptr(new Type(TypeTags::NONE, nullptr));
    anyType = Type::Ptr(new Type(TypeTags::NONE, nullptr));
    objectType = enterClass("java.lang.Object");
    classType = enterClass("java.lang.Class");
    stringType = enterClass("java.lang.String");

    printStreamType = enterClass("java.io.PrintStream");
    systemType = enterClass("java.lang.System");

    arrayClass = ClassSymbol::Ptr(new ClassSymbol(Flags::PUBLIC, *names.Array, nullptr));
    methodClass = ClassSymbol::Ptr(new ClassSymbol(Flags::PUBLIC, *names.METHOD, nullptr));
    noSymbol = TypeSymbol::Ptr(nullptr);

    initType(intType, "int");
    initType(booleanType, "boolean");
    initType(botType, "null");
    initType(voidType, "void");
}

Type::Ptr Symtab::enterClass(const string& fullName) {
    Name& name = names.fromString(fullName);
    return reader.enterClass(name)->type;
}

void Symtab::initType(Type::Ptr& type, std::string name) {
    TypeSymbol::Ptr sym(new ClassSymbol(
            Flags::PUBLIC, names.fromString(name), type, nullptr));
    noRootSymbols.push_back(sym);
    typeOfTag[type->tag] = type;
    type->tsym = sym;
}
