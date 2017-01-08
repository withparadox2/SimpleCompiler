//
// Created by withparadox2 on 2016/12/19.
//

#include "type.h"
#include "TypeTags.h"
#include "symbol.h"

ClassType::ClassType(TypeSymbol::Ptr tsym) : Type(TypeTags::CLASS, tsym) {

}

Type::Type(int tag, TypeSymbol::Ptr tsym) : tag(tag), tsym(tsym) {

}

MethodType::MethodType(vector<Type::Ptr> argtypes, Type::Ptr restype, TypeSymbol::Ptr methodClass)
        : Type(TypeTags::METHOD, methodClass), argtypes(argtypes), restype(restype) {

}
