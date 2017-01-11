//
// Created by withparadox2 on 2016/12/19.
//

#include "symbol.h"
#include "type.h"

namespace Kind {
    int PKG = 1 << 0;
/**type symbols (classes, interfaces and type variables).*/
    int TYP = 1 << 1;
/**variable symbols.*/
    int VAR = 1 << 2;
/**values (variables or non-variable expressions), includes VAR.*/
    int VAL = (1 << 3) | VAR;
/**methods*/
    int MTH = (1 << 4);
/**The error kind, which includes all other kinds.*/
    int ERR = (1 << 5);
};


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

ScopePtr ClassSymbol::member() {
    if (completer != nullptr) {
        completer->complete(shared_from_this());
        completer = nullptr;
    }
    return memberField;
}

TypeSymbol::TypeSymbol(long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : Symbol(Kind::TYP, flags, name, type, owner) {
}

Symbol::Symbol(int kind, long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : kind(kind), flags(flags), name(name), type(type), owner(owner), completer(nullptr) {

}

VarSymbol::VarSymbol(long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : Symbol(Kind::VAR, flags, name, type, owner) {

}

MethodSymbol::MethodSymbol(long flags, const Name& name, Type::Ptr type, Symbol::Ptr owner)
        : Symbol(Kind::MTH, flags, name, type, owner) {
}