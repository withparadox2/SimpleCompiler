//
// Created by withparadox2 on 2016/12/25.
//

#include "ClassReader.h"

using std::make_pair;

ClassReader& ClassReader::instance() {
    static ClassReader reader;
    return reader;
}


ClassSymbol::Ptr& ClassReader::enterClass(const Name& name) {
    auto iter = classes.find(&name);
    if (iter == classes.end()) {
        ClassSymbol::Ptr symbol(defineClass(name));
        classes.insert(make_pair(&name, symbol));
    }
    return classes.at(&name);
}

ClassSymbol* ClassReader::defineClass(const Name& name) {
    ClassSymbol* symbol = new ClassSymbol(0, name, nullptr);
    return symbol;
}
