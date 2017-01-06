//
// Created by withparadox2 on 2016/12/19.
//

#include "scope.h"

void Scope::enter(Symbol::Ptr symbol) {
    if(nameToSym.find(&symbol->name) == nameToSym.end()) {
        nameToSym.insert(std::make_pair(&symbol->name, symbol));
    }
}

Scope::Scope(Symbol::Ptr owner) : owner(owner){
}
