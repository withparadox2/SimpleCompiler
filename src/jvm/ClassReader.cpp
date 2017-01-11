//
// Created by withparadox2 on 2016/12/25.
//

#include "ClassReader.h"
#include "../util/error.h"
#include <cstddef>
#include "../code/scope.h"
#include "../code/type.h"
#include "../code/Symtab.h"
#include "../util/tools.h"

using std::vector;

ClassReader& ClassReader::instance() {
    static ClassReader reader;
    return reader;
}

ClassSymbol::Ptr& ClassReader::enterClass(const Name& flatName) {
    auto iter = classes.find(&flatName);
    if (iter != classes.end()) {
        error(flatName.desc + " can not enter twice");
    }

    //default means symbol is under package of java.lang
    bool isDefault = flatName.desc.find_last_of('.') != string::npos;

    ClassSymbol::Ptr symbol(defineClass(shortName(flatName)));
    symbol->initOnShared();
    if (isDefault) {
        complete(symbol);
    }
    classes.insert(std::make_pair(&flatName, symbol));
    return classes.at(&flatName);
}

ClassSymbol* ClassReader::defineClass(const Name& name) {
    ClassSymbol* symbol = new ClassSymbol(0, name, nullptr);
    return symbol;
}

void ClassReader::complete(ClassSymbol::Ptr& sym) {
    Names& names = sym->name.names;
    sym->memberField = Scope::Ptr(new Scope(sym));
    if (sym->name == names.fromString("System")) {
        VarSymbol::Ptr outSym(
                new VarSymbol(0,
                              names.fromString("out"),
                              classes.at(&names.fromString("java.io.PrintStream"))->type,
                              sym));
        sym->memberField->enter(outSym);
    } else if (sym->name == names.fromString("PrintStream")) {
        vector<TypePtr> args;
        args.push_back(TypePtr(classes.at(&names.fromString("java.lang.String"))->type));
        MethodType::Ptr printType(new MethodType(args, Symtab::instance().voidType, sym));

        MethodSymbol::Ptr printSym(
                new MethodSymbol(0, names.fromString("println"), printType, sym));
        sym->memberField->enter(printSym);
    } else if (sym->name == names.fromString("String")) {
    }
}
