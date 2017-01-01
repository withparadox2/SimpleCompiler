//
// Created by withparadox2 on 2016/12/19.
//

#include "type.h"
#include "TypeTags.h"

ClassType::ClassType(TypeSymbol *tsym) : Type(TypeTags::CLASS, tsym) {

}

Type::Type(int tag, TypeSymbol *tsym) : tag(tag), tsym(tsym){

}
