//
// Created by withparadox2 on 2016/12/25.
//

#include "ClassReader.h"
#include "../util/error.h"
#include <cstddef>

const Name& shortName(const Name& src) {
    size_t pos = src.desc.find_last_of('.');
    if (pos == std::string::npos) {
        return src;
    }

    Name& shortName = Names::instance().fromString(src.desc.substr(pos + 1));
    return shortName;
}

ClassReader& ClassReader::instance() {
    static ClassReader reader;
    return reader;
}

ClassSymbol::Ptr& ClassReader::enterClass(const Name& flatName) {
    auto iter = classes.find(&flatName);
    if (iter != classes.end()) {
        error(flatName.desc + " can not enter twice");
    }

    //default means symbol is under package of java.lang
    bool isDefault = flatName.desc.find_last_of('.') != string::npos;

    ClassSymbol::Ptr symbol(defineClass(shortName(flatName)));
    symbol->initOnShared();
    if (isDefault) {
        complete(symbol);
    }
    classes.insert(std::make_pair(&flatName, symbol));
    return classes.at(&flatName);
}

ClassSymbol* ClassReader::defineClass(const Name& name) {
    ClassSymbol* symbol = new ClassSymbol(0, name, nullptr);
    return symbol;
}

void ClassReader::complete(ClassSymbol::Ptr& sym) {
    Names& names = sym->name.names;
}
