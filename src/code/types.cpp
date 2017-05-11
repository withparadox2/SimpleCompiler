//
// Created by withparadox2 on 2017/2/7.
//

#include "types.h"
#include "type.h"
#include "Symtab.h"

int Types::dimensions(TypePtr t) {
    int result = 0;
    while (t->tag == TypeTags::ARRAY) {
        result++;
        t = elemtype(t);
    }
    return result;
}

TypePtr Types::elemtype(TypePtr t) {
    if (t->tag == TypeTags::ARRAY) {
        return std::dynamic_pointer_cast<ArrayType>(t)->elemtype;
    }
    return Symtab::instance().errorType;
}
