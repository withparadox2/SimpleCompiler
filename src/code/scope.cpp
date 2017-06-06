//
// Created by withparadox2 on 2016/12/19.
//

#include "scope.h"
#include "Symtab.h"
#include "../jvm/ClassReader.h"
#include "../util/log.h"

SymbolList Scope::emptyList;

void Scope::enter(SymbolPtr symbol) {
    log("enter symbol: " + symbol->name.desc);
    if (nameToSym->find(&symbol->name) == nameToSym->end()) {
        nameToSym->insert(std::make_pair(&symbol->name, SymbolList()));
    }

    SymbolList& list = nameToSym->at(&symbol->name);
    list.push_back(symbol);
    elems.insert(&symbol->name);
}

void Scope::remove(const Name& name) {
    SymbolPtr& sym = lookUp(name);
    if (sym == Symtab::instance().noSymbol) {
        return;
    }
    nameToSym->erase(&name);
    elems.erase(&name);
}

Scope::Scope(SymbolPtr owner) : owner(owner), nameToSym(new Map) {
}

SymbolPtr& Scope::lookUp(const Name& name) {
    if (nameToSym->find(&name) != nameToSym->end()) {
        SymbolList& list = nameToSym->at(&name);
        if (list.size() > 0) {
            return list.at(0);
        }
    }
    return Symtab::instance().noSymbol;
}

SymbolList& Scope::lookUpList(const Name& name) {
    if (nameToSym->find(&name) != nameToSym->end()) {
        return nameToSym->at(&name);
    }
    return emptyList;
}

Scope::Ptr Scope::dupUnshared() {
    MapPtr nameToSym(new Map(*this->nameToSym));
    return Scope::Ptr(new Scope(*this, nameToSym));
}

Scope::Ptr Scope::dup() {
    return Scope::Ptr(new Scope(*this, this->nameToSym));
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

void Scope::printTable() {
    log("==========begin table==========");
    for (auto iter = nameToSym->begin(); iter != nameToSym->end(); iter++) {
        log(iter->first->desc);
    }
    log("==========end table==========");
}

StarImportScope::StarImportScope() : Scope(Symtab::instance().noSymbol) {
    importAll();
}

void StarImportScope::importAll() {
    ClassReader& reader = ClassReader::instance();
    Names& names = Names::instance();
    enter(reader.enterClass(names.fromString("java.lang.String")));
    enter(reader.enterClass(names.fromString("java.io.PrintStream")));
    enter(reader.enterClass(names.fromString("java.lang.System")));
    enter(reader.enterClass(names.fromString("java.lang.Object")));
}

StarImportScope& StarImportScope::instance() {
    static StarImportScope instance;
    return instance;
}

