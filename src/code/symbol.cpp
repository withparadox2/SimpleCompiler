//
// Created by withparadox2 on 2016/12/19.
//

#include "symbol.h"

ClassSymbol::ClassSymbol(long flags, const Name &name, Symbol *owner)
        : ClassSymbol(flags, name, new Type, owner) {
    this->type->tsym = this;
}

ClassSymbol::ClassSymbol(long flags, const Name &name, Type *type, Symbol *owner)
        : TypeSymbol(flags, name, type, owner) {
}

TypeSymbol::TypeSymbol(long flags, const Name &name, Type *type, Symbol *owner)
        : Symbol(1, flags, name, type, owner) {
}

Symbol::Symbol(int kind, long flags, const Name &name, Type *type, Symbol *owner)
        : kind(kind), flags(flags), name(name), type(type), owner(owner) {
}
