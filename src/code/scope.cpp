//
// Created by withparadox2 on 2016/12/19.
//

#include "scope.h"

void Scope::enter(Symbol::Ptr symbol) {
    if (nameToSym->find(&symbol->name) == nameToSym->end()) {
        nameToSym->insert(std::make_pair(&symbol->name, symbol));
        elems.insert(&symbol->name);
    }
}

void Scope::remove(const Name& name) {
    Symbol::Ptr& sym = lookUp(name);
    if (sym == Symtab::instance().noSymbol) {
        return;
    }
    nameToSym->erase(&name);
    elems.erase(&name);
}

Scope::Scope(Symbol::Ptr owner) : owner(owner), nameToSym(new Map) {
}

Symbol::Ptr& Scope::lookUp(const Name& name) {
    if (nameToSym->find(&name) != nameToSym->end()) {
        return nameToSym->at(&name);
    }
    return Symtab::instance().noSymbol;
}

Scope* Scope::dupUnshared() {
    MapPtr nameToSym(new Map(*this->nameToSym));
    return new Scope(*this, nameToSym);
}

Scope* Scope::dup() {
    return new Scope(*this, this->nameToSym);
}

Scope::Scope(const Scope& c, const MapPtr& nameToSym) :
        next(const_cast<Scope&>(c).shared_from_this()),
        owner(c.owner.lock()),
        nameToSym(nameToSym) {
}

Scope::Ptr Scope::leave() {
    if (nameToSym.get() != next->nameToSym.get()) {
        return next;
    }
    for (auto iter = elems.begin(); iter != elems.end(); iter++) {
        nameToSym->erase(*iter);
    }
    elems.clear();
    return next;
}


