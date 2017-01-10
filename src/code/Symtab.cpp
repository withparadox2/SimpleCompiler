//
// Created by withparadox2 on 2016/12/31.
//

#include "Symtab.h"
#include "../code/TypeTags.h"

Symtab& Symtab::instance() {
    static Symtab inst;
    return inst;
}

Symtab::Symtab() : reader(ClassReader::instance()), names(Names::instance()) {
    voidType = Type::Ptr(new Type(TypeTags::VOID, nullptr));
    objectType = enterClass("java.lang.Object");
    classType = enterClass("java.lang.Class");
    stringType = enterClass("java.lang.String");

    //PrintStream must before System, TODO correct it.
    printStreamType = enterClass("java.io.PrintStream");
    systemType = enterClass("java.lang.System");

    arrayClass = ClassSymbol::Ptr(new ClassSymbol(Flags::PUBLIC, *names.Array, nullptr));
    noSymbol = TypeSymbol::Ptr(nullptr);
}

Type::Ptr Symtab::enterClass(const string& fullName) {
    Name& name = names.fromString(fullName);
    return reader.enterClass(name)->type;
}