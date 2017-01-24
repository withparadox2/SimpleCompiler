//
// Created by withparadox2 on 2016/12/19.
//

#include "type.h"
#include "TypeTags.h"

ClassType::ClassType(TypeSymbolPtr tsym) : Type(TypeTags::CLASS, tsym) {

}

Type::Type(int tag, TypeSymbolPtr tsym) : tag(tag), tsym(tsym) {

}

TypePtr Type::getReturnType() {
    return nullptr;
}

TypeList Type::getParameterTypes() {
    return TypeList();
}

MethodType::MethodType(vector<TypePtr> argtypes, TypePtr restype, TypeSymbolPtr methodClass)
        : Type(TypeTags::METHOD, methodClass), argtypes(argtypes), restype(restype) {

}

TypePtr MethodType::getReturnType() {
    return restype;
}

TypeList MethodType::getParameterTypes() {
    return argtypes;
}

ArrayType::ArrayType(TypePtr elemtype, TypeSymbolPtr arrayClass)
        : Type(TypeTags::ARRAY, arrayClass), elemtype(elemtype) {
}
