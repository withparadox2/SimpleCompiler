//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SCOPE_H
#define SIMPLECOMPILER_SCOPE_H

#include <map>
#include "./symbol.h"

using std::map;

class Scope {
private:
    map<const Name *, Symbol::Ptr> nameToSym;
public:
    typedef std::shared_ptr<Scope> Ptr;

    Symbol::WeakPtr owner;
    void enter(Symbol::Ptr symbol);
    Scope(Symbol::Ptr owner);

};


#endif //SIMPLECOMPILER_SCOPE_H
