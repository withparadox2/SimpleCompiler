//
// Created by withparadox2 on 2016/12/31.
//

#include "Symtab.h"
#include "type.h"
#include "../jvm/ClassReader.h"
#include "../comp/env.h"
#include "../util/tools.h"
#include "../jvm/bytecode.h"

Symtab& Symtab::instance() {
    static Symtab inst;
    return inst;
}

Symtab::Symtab() : reader(ClassReader::instance()), names(Names::instance()), typeOfTag{Type::Ptr()} {
    predefClass = ClassSymbolPtr(new ClassSymbol(Flags::PUBLIC, *names.empty, nullptr));
    ScopePtr scope(new Scope(predefClass));
    predefClass->memberField = scope;

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

    enterUnop("+", intType, intType, bytecode::nop);
    enterUnop("-", intType, intType, bytecode::nop);
    enterUnop("!", booleanType, booleanType, bytecode::bool_not);

    enterBinop("+", stringType, intType, stringType, bytecode::string_add);
    enterBinop("+", intType, stringType, stringType, bytecode::string_add);
    enterBinop("+", stringType, booleanType, stringType, bytecode::string_add);
    enterBinop("+", booleanType, stringType, stringType, bytecode::string_add);

    enterBinop("+", intType, intType, intType, bytecode::iadd);
    enterBinop("-", intType, intType, intType, bytecode::isub);
    enterBinop("*", intType, intType, intType, bytecode::imul);
    enterBinop("/", intType, intType, intType, bytecode::idiv);
    enterBinop("%", intType, intType, intType, bytecode::imod);
}

Type::Ptr Symtab::enterClass(const string& fullName) {
    Name& name = names.fromString(fullName);
    return reader.enterClass(name)->type;
}

void Symtab::initType(Type::Ptr& type, std::string name) {
    TypeSymbol::Ptr sym(new ClassSymbol(
            Flags::PUBLIC, names.fromString(name), type, nullptr));
    typeOfTag[type->tag] = type;
    type->tsym = sym;

    predefClass->memberField->enter(sym);
}

void Symtab::enterUnop(std::string name, TypePtr arg, TypePtr res, int opcode) {

    predefClass->memberField->enter(
            OperatorSymbol::Ptr(
                    new OperatorSymbol(names.fromString(name),
                                       MethodType::Ptr(new MethodType(ofList(arg), res, methodClass)),
                                       opcode, predefClass)));
}

void Symtab::enterBinop(std::string name, TypePtr left, TypePtr right, TypePtr res, int opcode) {
    predefClass->memberField->enter(
            OperatorSymbol::Ptr(
                    new OperatorSymbol(names.fromString(name),
                                       MethodType::Ptr(new MethodType(ofList(left, right), res, methodClass)),
                                       opcode, predefClass)));
}
