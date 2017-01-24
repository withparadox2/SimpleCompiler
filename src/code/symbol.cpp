//
// Created by withparadox2 on 2016/12/19.
//

#include "symbol.h"
#include "type.h"
#include "Flags.h"

ClassSymbol::ClassSymbol(long flags, const Name& name, SymbolPtr owner)
        : ClassSymbol(flags, name, TypePtr(new ClassType(nullptr)), owner) {
}

ClassSymbol::ClassSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner)
        : TypeSymbol(flags, name, type, owner), memberField(nullptr) {
}

/**
 * Since this meth calls shared_from_this(), it can not be inserted into a constructor.
 */
SymbolPtr ClassSymbol::initOnShared() {
    TypeSymbolPtr ptr = std::dynamic_pointer_cast<TypeSymbol>(shared_from_this());
    this->type->tsym = ptr;
    return shared_from_this();
}

ScopePtr ClassSymbol::member() {
    if (completer != nullptr) {
        completer->complete(shared_from_this());
        completer = nullptr;
    }
    return memberField;
}

TypeSymbol::TypeSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner)
        : Symbol(Kind::TYP, flags, name, type, owner) {
}

Symbol::Symbol(int kind, long flags, const Name& name, TypePtr type, SymbolPtr owner)
        : kind(kind), flags(flags), name(name), type(type), owner(owner), completer(nullptr) {

}

ScopePtr Symbol::member() {
    return ScopePtr();
}

VarSymbol::VarSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner)
        : Symbol(Kind::VAR, flags, name, type, owner) {

}

MethodSymbol::MethodSymbol(long flags, const Name& name, TypePtr type, SymbolPtr owner)
        : Symbol(Kind::MTH, flags, name, type, owner) {
}

OperatorSymbol::OperatorSymbol(const Name& name, TypePtr type, int opcode, SymbolPtr owner)
        : MethodSymbol(Flags::PUBLIC | Flags::STATIC, name, type, owner), opcode(opcode) {
}
