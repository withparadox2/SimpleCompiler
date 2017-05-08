//
// Created by withparadox2 on 2017/2/7.
//

#include "types.h"
#include "type.h"
#include "TypeTags.h"

int Types::dimensions(TypePtr t) {
    int result = 0;
    while (t->tag == TypeTags::ARRAY) {
        result++;
        t = std::dynamic_pointer_cast<ArrayType>(t)->elemtype;
    }
    return result;
}