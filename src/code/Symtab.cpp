//
// Created by withparadox2 on 2016/12/31.
//

#include "Symtab.h"
#include "type.h"
#include "../jvm/ClassReader.h"
#include "../comp/env.h"
#include "../util/tools.h"
#include "../jvm/bytecode.h"

#define KEY_SYMTAB "symtab"

Symtab& Symtab::instance() {
    Symtab* inst = Context::instance().get<Symtab>(KEY_SYMTAB);
    if (inst == nullptr) {
        inst = new Symtab();
    }
    return *inst;
}

Symtab::Symtab() : typeOfTag{TypePtr()} {
    Context::instance().put(KEY_SYMTAB, this);
    reader = &ClassReader::instance();
    names = &Names::instance();
    predefClass = ClassSymbolPtr(new ClassSymbol(Flags::PUBLIC, *names->empty, nullptr));
    predefClass->initOnShared();

    ScopePtr scope(new Scope(predefClass));
    predefClass->memberField = scope;

    intType = TypePtr(new Type(TypeTags::INT, nullptr));
    booleanType = TypePtr(new Type(TypeTags::BOOLEAN, nullptr));
    botType = TypePtr(new Type(TypeTags::BOT, nullptr));
    voidType = TypePtr(new Type(TypeTags::VOID, nullptr));
    unknownType = TypePtr(new Type(TypeTags::UNKNOWN, nullptr));
    noType = TypePtr(new Type(TypeTags::NONE, nullptr));
    anyType = TypePtr(new Type(TypeTags::NONE, nullptr));
    errorType = TypePtr(new Type(TypeTags::ERROR, nullptr));
    objectType = enterClass("java.lang.Object");
    classType = enterClass("java.lang.Class");
    stringType = enterClass("java.lang.String");

    printStreamType = enterClass("java.io.PrintStream");
    systemType = enterClass("java.lang.System");
    stringBuilderType = enterClass("java.lang.StringBuilder");

    arrayClass = ClassSymbolPtr(new ClassSymbol(Flags::PUBLIC, *names->Array, nullptr));
    methodClass = ClassSymbolPtr(new ClassSymbol(Flags::PUBLIC, *names->METHOD, nullptr));
    noSymbol = TypeSymbolPtr(nullptr);


    setUpArrayType(arrayClass);

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

    enterBinop("<", intType, intType, booleanType, bytecode::if_icmplt);
    enterBinop(">", intType, intType, booleanType, bytecode::if_icmpgt);
    enterBinop("<=", intType, intType, booleanType, bytecode::if_icmple);
    enterBinop(">=", intType, intType, booleanType, bytecode::if_icmpge);
    enterBinop("==", intType, intType, booleanType, bytecode::if_icmpeq);
    enterBinop("!=", intType, intType, booleanType, bytecode::if_icmpne);
}


TypePtr Symtab::enterClass(const string& fullName) {
    Name& name = names->fromString(fullName);
    return reader->enterClass(name)->type;
}

void Symtab::initType(TypePtr& type, std::string name) {
    TypeSymbolPtr sym(new ClassSymbol(
            Flags::PUBLIC, names->fromString(name), type, nullptr));
    typeOfTag[type->tag] = type;
    type->tsym = sym;

    predefClass->memberField->enter(sym);
}

void Symtab::enterUnop(std::string name, TypePtr arg, TypePtr res, int opcode) {

    predefClass->memberField->enter(
            OperatorSymbolPtr(
                    new OperatorSymbol(
                            names->fromString(name),
                            MethodTypePtr(new MethodType(ofList(arg), res, methodClass)),
                            opcode, predefClass)));
}

void Symtab::enterBinop(std::string name, TypePtr left, TypePtr right, TypePtr res, int opcode) {
    predefClass->memberField->enter(
            OperatorSymbolPtr(
                    new OperatorSymbol(
                            names->fromString(name),
                            MethodTypePtr(new MethodType(ofList(left, right), res, methodClass)),
                            opcode, predefClass)));
}

void Symtab::setUpArrayType(ClassSymbolPtr sym) {
    ClassTypePtr arrayClassType =
            std::dynamic_pointer_cast<ClassType>(sym->type);
    arrayClassType->supertype_field = objectType;
    sym->memberField = Scope::Ptr(new Scope(sym));
    this->lengthVar = VarSymbolPtr(
            new VarSymbol(Flags::PUBLIC | Flags::FINAL,
                          names->fromString("length"),
                          intType,
                          sym));
    sym->memberField->enter(this->lengthVar);

}
