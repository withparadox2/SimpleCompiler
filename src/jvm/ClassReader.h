//
// Created by withparadox2 on 2016/12/25.
//

#ifndef SIMPLECOMPILER_CLASSREADER_H
#define SIMPLECOMPILER_CLASSREADER_H

#include <map>
#include "../code/symbols.h"
#include "../comp/completer.h"
#include "../util/context.h"
#include "../code/types.h"

class Name;

class ClassReader : public Completer, public EnableMapHelper {
private:
    std::map<const Name*, ClassSymbolPtr> classes;

    void insertAppendMethod(ClassSymbolPtr sym, TypePtr type);

    void insertDefaultInit(ClassSymbolPtr sym);
public:
    static ClassReader& instance();

    /**There exists no concept about package.*/
    ClassSymbolPtr& enterClass(const Name& flatName);

    /**
     * Returns a raw pointer of class symbol, and it's necessary
     * to call initOnShared() after being wrapped into a shared_pointer.
     */
    ClassSymbol* defineClass(const Name& name);

    void complete(SymbolPtr sym);
};


#endif //SIMPLECOMPILER_CLASSREADER_H