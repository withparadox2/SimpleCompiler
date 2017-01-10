//
// Created by withparadox2 on 2016/12/25.
//

#ifndef SIMPLECOMPILER_CLASSREADER_H
#define SIMPLECOMPILER_CLASSREADER_H

#include <map>
#include "../code/symbol.h"

using std::map;

class ClassReader {
private:
    map<const Name*, ClassSymbol::Ptr> classes;
    void complete(ClassSymbol::Ptr& sym);
public:
    static ClassReader& instance();

    //there exists no concept about package.
    ClassSymbol::Ptr& enterClass(const Name& flatName);

    ClassSymbol* defineClass(const Name& name);

};


#endif //SIMPLECOMPILER_CLASSREADER_H