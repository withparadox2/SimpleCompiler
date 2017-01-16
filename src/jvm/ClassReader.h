//
// Created by withparadox2 on 2016/12/25.
//

#ifndef SIMPLECOMPILER_CLASSREADER_H
#define SIMPLECOMPILER_CLASSREADER_H

#include <map>
#include "../code/symbols.h"
#include "../comp/completer.h"

using std::map;
class Name;

class ClassReader : public Completer {
private:
    map<const Name*, ClassSymbolPtr> classes;

public:
    static ClassReader& instance();

    //there exists no concept about package.
    ClassSymbolPtr& enterClass(const Name& flatName);

    ClassSymbol* defineClass(const Name& name);

    void complete(SymbolPtr sym);
};


#endif //SIMPLECOMPILER_CLASSREADER_H