//
// Created by withparadox2 on 2016/12/19.
//

#include "type.h"
#include "TypeTags.h"

ClassType::ClassType(TypeSymbol::Ptr tsym) : Type(TypeTags::CLASS, tsym) {

}

Type::Type(int tag, TypeSymbol::Ptr tsym) : tag(tag), tsym(tsym) {

}

TypePtr Type::getReturnType() {
    return nullptr;
}

Type::List Type::getParameterTypes() {
    return Type::List();
}

MethodType::MethodType(vector<Type::Ptr> argtypes, Type::Ptr restype, TypeSymbol::Ptr methodClass)
        : Type(TypeTags::METHOD, methodClass), argtypes(argtypes), restype(restype) {

}

TypePtr MethodType::getReturnType() {
    return restype;
}

Type::List MethodType::getParameterTypes() {
    return argtypes;
}

ArrayType::ArrayType(Type::Ptr elemtype, TypeSymbol::Ptr arrayClass)
        : Type(TypeTags::ARRAY, arrayClass), elemtype(elemtype) {
}
