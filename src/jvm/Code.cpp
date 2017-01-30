//
// Created by withparadox2 on 2017/1/27.
//

#include "Code.h"
#include "../code/type.h"
#include "../code/TypeTags.h"
#include "bytecode.h"
#include "../util/error.h"

int Code::typecode(const Type* type) {
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
    int reg = newLocal(v->type.get());
    v->adr = reg;
    addLocalVar(v);
    return 0;
}

void Code::setDefined(int adr) {

}

void Code::emitop0(int op) {

}

void Code::emitop(int op) {

}

int Code::truncate(int typecode) {
    using namespace bytecode;
    switch (typecode) {
        case BYTEcode:
        case SHORTcode:
        case CHARcode:
            return INTcode;
        default:
            return typecode;
    }
}

int Code::newLocal(const Type* type) {
    return newLocal(typecode(type));
}

int Code::newLocal(int typecode) {
    int reg = nextreg;
    int w = width(typecode);
    nextreg = reg + w;
    if (nextreg > max_locals) {
        max_locals = nextreg;
    }
    return reg;
}

Code::Code() : nextreg(0) {
}

int Code::width(int typecode) {
    switch (typecode) {
        case bytecode::LONGcode:
        case bytecode::DOUBLEcode:
            return 2;
        case bytecode::VOIDcode:
            //TODO Figures out how to store void type
            return 0;
        default:
            return 1;
    }
}

int Code::width(const Type* type) {
    return type == nullptr ? 1 : width(typecode(type));
}

void Code::addLocalVar(VarSymbolPtr v) {
    int adr = v->adr;
    if (adr + 1 >= lvar.capacity()) {
        std::vector::size_type newLen
                = lvar.capacity() << 1;
        if (newLen <= adr) {
            newLen = (vector::size_type) (adr + 10);
        }
        lvar.reserve(newLen);
    }
    //TODO if (pendingJumps != null) resolvePending();
    lvar[adr] = LocalVar::Ptr(new LocalVar(v));
    //TODO state.defined.excl(adr);
}

void Code::emitop2(int op, int od) {

}

void Code::emitop1w(int op, int od1, int od2) {

}

void Code::emitop1w(int op, int od) {

}

LocalVar::LocalVar(VarSymbolPtr v)
        : sym(v),
          reg(static_cast<char16_t>(v->adr)),
          start_pc(0xffff),
          length(0xffff) {
}

LocalVar* LocalVar::dup() {
    return new LocalVar(sym);
}
