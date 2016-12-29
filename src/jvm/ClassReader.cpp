//
// Created by withparadox2 on 2016/12/25.
//

#include "ClassReader.h"
using std::make_pair;

ClassReader &ClassReader::instance() {
    static ClassReader reader;
    return reader;
}


ClassSymbol *ClassReader::enterClass(const Name &name) {
    auto iter = classes.find(&name);
    ClassSymbol *symbol = iter == classes.end() ? nullptr : iter->second;
    if (symbol == nullptr) {
        symbol = defineClass(name);
        classes.insert(make_pair(&name, symbol));
    }
    return symbol;
}

ClassSymbol *ClassReader::defineClass(const Name &name) {
    ClassSymbol *symbol = new ClassSymbol(0, name, nullptr);
    return symbol;
}
