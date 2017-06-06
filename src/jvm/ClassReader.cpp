//
// Created by withparadox2 on 2016/12/25.
//

#include "ClassReader.h"
#include <cstddef>
#include "../util/error.h"
#include "../code/scope.h"
#include "../code/type.h"
#include "../code/Symtab.h"
#include "../util/tools.h"

using std::vector;

#define KEY_CLASS_READER "class_reader"

ClassReader& ClassReader::instance() {
    ClassReader* inst = Context::instance()
            .get<ClassReader>(KEY_CLASS_READER);
    if (inst == nullptr) {
        inst = new ClassReader();
    }
    return *inst;
}

ClassSymbolPtr& ClassReader::enterClass(const Name& flatName) {
    auto iter = classes.find(&flatName);
    if (iter != classes.end()) {
        return classes.at(&flatName);
    }

    //default means symbol is under package of java.lang
    bool isDefault = flatName.desc.find_last_of('.') != string::npos;

    ClassSymbolPtr symbol(defineClass(shortName(flatName)));
    symbol->initOnShared();
    if (isDefault) {
        symbol->completer = this;
    }
    classes.insert(std::make_pair(&flatName, symbol));
    return classes.at(&flatName);
}

ClassSymbol* ClassReader::defineClass(const Name& name) {
    ClassSymbol* symbol = new ClassSymbol(0, name, nullptr);
    return symbol;
}

void ClassReader::complete(SymbolPtr symP) {
    ClassSymbolPtr sym = std::dynamic_pointer_cast<ClassSymbol>(symP);
    Names& names = sym->name.names;
    sym->memberField = Scope::Ptr(new Scope(sym));
    if (sym->name == names.fromString("System")) {
        VarSymbolPtr outSym(
                new VarSymbol(0,
                              names.fromString("out"),
                              classes.at(&names.fromString("java.io.PrintStream"))->type,
                              sym));
        sym->memberField->enter(outSym);
    } else if (sym->name == names.fromString("PrintStream")) {
        vector<TypePtr> args;
        args.push_back(classes.at(&names.fromString("java.lang.String"))->type);
        MethodTypePtr printType(new MethodType(args, Symtab::instance().voidType, sym));

        MethodSymbolPtr printSym(
                new MethodSymbol(0, names.fromString("println"), printType, sym));
        sym->memberField->enter(printSym);
    } else if (sym->name == names.fromString("String")) {
    } else if (sym->name == names.fromString("Object")) {
        MethodTypePtr type(new MethodType(TypeList(), Symtab::instance().voidType, sym));
        MethodSymbolPtr printSym(
                new MethodSymbol(0, *names.init, type, sym));
        sym->memberField->enter(printSym);
    } else if (sym->name == names.fromString("StringBuilder")) {
        insertAppendMethod(sym, Symtab::instance().stringType);
        insertAppendMethod(sym, Symtab::instance().intType);
        insertDefaultInit(sym);

        MethodTypePtr type(new MethodType(TypeList(), Symtab::instance().stringType, sym));
        MethodSymbolPtr toStringSym(
                new MethodSymbol(0, *sym->name.names.toString, type, sym));
        sym->memberField->enter(toStringSym);
    }
}

void ClassReader::insertAppendMethod(ClassSymbolPtr sym, TypePtr type) {
    Names& names = sym->name.names;

    vector<TypePtr> args;
    args.push_back(type);
    MethodTypePtr appendType(new MethodType(args, Symtab::instance().stringBuilderType, sym));

    MethodSymbolPtr appendSym(
            new MethodSymbol(0, names.fromString("append"), appendType, sym));
    sym->memberField->enter(appendSym);
}

void ClassReader::insertDefaultInit(ClassSymbolPtr sym) {
    MethodTypePtr type(new MethodType(TypeList(), Symtab::instance().voidType, sym));
    MethodSymbolPtr initSym(
            new MethodSymbol(0, *sym->name.names.init, type, sym));
    sym->memberField->enter(initSym);
}