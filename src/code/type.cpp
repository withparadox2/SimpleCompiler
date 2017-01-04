//
// Created by withparadox2 on 2016/12/19.
//

#include "type.h"
#include "TypeTags.h"
#include "symbol.h"

ClassType::ClassType(TypeSymbol *tsym) : Type(TypeTags::CLASS, tsym) {

}

Type::Type(int tag, TypeSymbol *tsym) : tag(tag), tsym(tsym){

}

MethodType::MethodType(vector<Type *> *argtypes, Type *restype, TypeSymbol *methodClass)
        : Type(TypeTags::METHOD, methodClass), argtypes(argtypes), restype(restype) {

}
