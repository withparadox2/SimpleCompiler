//
// Created by withparadox2 on 2017/1/27.
//

#include "Code.h"
#include "../code/type.h"
#include "../code/TypeTags.h"
#include "bytecode.h"
#include "../util/error.h"

int Code::typecode(const TypePtr& type) {
    using namespace TypeTags;
    using namespace bytecode;

    switch (type->tag) {
        case BYTE:
            return BYTEcode;
        case SHORT:
            return SHORTcode;
        case CHAR:
            return CHARcode;
        case INT:
            return INTcode;
        case LONG:
            return LONGcode;
        case FLOAT:
            return FLOATcode;
        case DOUBLE:
            return DOUBLEcode;
        case BOOLEAN:
            return BYTEcode;
        case VOID:
            return VOIDcode;
        case CLASS:
        case ARRAY:
        case METHOD:
        case BOT:
        case TYPEVAR:
//        case UNINITIALIZED_THIS:
//        case UNINITIALIZED_OBJECT:
            return OBJECTcode;
        default:
            error("unknown typecode : " + type->tag);
    }
}

int Code::newLocal(VarSymbolPtr v) {
    return 0;
}

void Code::setDefined(int adr) {

}
