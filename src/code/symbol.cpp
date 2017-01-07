//
// Created by withparadox2 on 2016/12/19.
//

#include "symbol.h"
#include "type.h"

ClassSymbol::ClassSymbol(long flags, const Name& name, Symbol::Ptr owner)
        : ClassSymbol(flags, name, new ClassType(nullptr), owner) {
    this->type->tsym = this;
}

ClassSymbol::ClassSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner)
        : TypeSymbol(flags, name, type, owner), memberField(nullptr) {
}

TypeSymbol::TypeSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner)
        : Symbol(Kind::TYP, flags, name, type, owner) {
}

Symbol::Symbol(Kind kind, long flags, const Name& name, Type* type, Symbol::Ptr owner)
        : kindinternal(kind), flags(flags), name(name), type(type), owner(owner) {
}

int Symbol::kind() {
    return static_cast<int>(kindinternal);
}

VarSymbol::VarSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner)
        : Symbol(Kind::VAR, flags, name, type, owner) {

}

MethodSymbol::MethodSymbol(long flags, const Name& name, Type* type, Symbol::Ptr owner)
        : Symbol(Kind::MTH, flags, name, type, owner) {
}
