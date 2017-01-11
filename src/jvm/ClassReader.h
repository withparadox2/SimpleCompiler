//
// Created by withparadox2 on 2016/12/25.
//

#ifndef SIMPLECOMPILER_CLASSREADER_H
#define SIMPLECOMPILER_CLASSREADER_H

#include <map>
#include "../code/symbol.h"
#include "../comp/completer.h"

using std::map;

class ClassReader : public Completer {
private:
    map<const Name*, ClassSymbol::Ptr> classes;

public:
    static ClassReader& instance();

    //there exists no concept about package.
    ClassSymbol::Ptr& enterClass(const Name& flatName);

    ClassSymbol* defineClass(const Name& name);

    void complete(Symbol::Ptr sym);
};


#endif //SIMPLECOMPILER_CLASSREADER_H