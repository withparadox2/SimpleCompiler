//
// Created by withparadox2 on 2016/12/31.
//

#include "Symtab.h"

Symtab& Symtab::instance() {
    static Symtab inst;
    return inst;
}

Symtab::Symtab() : reader(ClassReader::instance()), names(Names::instance()) {
    objectType = enterClass("java.lang.Object");
    classType = enterClass("java.lang.Class");
    stringType = enterClass("java.lang.String");
    systemType = enterClass("java.lang.System");

    arrayClass = ClassSymbol::Ptr(new ClassSymbol(Flags::PUBLIC, *names.Array, nullptr));
}

Type::Ptr Symtab::enterClass(const string& fullName) {
    Name& name = names.fromString(fullName);
    return reader.enterClass(name)->type;
}
