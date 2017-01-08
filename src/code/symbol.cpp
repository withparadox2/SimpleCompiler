//
// Created by withparadox2 on 2016/12/19.
//

#include "symbol.h"
#include "type.h"

ClassSymbol::ClassSymbol(long flags, const Name& name, Symbol::Ptr owner)
        : ClassSymbol(flags, name, TypePtr(new ClassType(nullptr)), owner) {
}

ClassSymbol::ClassSymbol(long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : TypeSymbol(flags, name, type, owner), memberField(nullptr) {
}

/**
 * Since this meth calls shared_from_this, it can not be inserted into a constructor.
 */
void ClassSymbol::initOnShared() {
    TypeSymbol::Ptr ptr = std::static_pointer_cast<TypeSymbol>(shared_from_this());
    this->type->tsym = ptr;
}

TypeSymbol::TypeSymbol(long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : Symbol(Kind::TYP, flags, name, type, owner) {
}

Symbol::Symbol(Kind kind, long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : kindinternal(kind), flags(flags), name(name), type(type), owner(owner) {

}

int Symbol::kind() {
    return static_cast<int>(kindinternal);
}


VarSymbol::VarSymbol(long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : Symbol(Kind::VAR, flags, name, type, owner) {

}

MethodSymbol::MethodSymbol(long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : Symbol(Kind::MTH, flags, name, type, owner) {
}
