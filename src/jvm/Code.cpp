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


int Code::arraycode(const Type* type) {
    using namespace TypeTags;

    switch (type->tag) {
        case BYTE:
            return 8;
        case BOOLEAN:
            return 4;
        case SHORT:
            return 9;
        case CHAR:
            return 5;
        case INT:
            return 10;
        case LONG:
            return 11;
        case FLOAT:
            return 6;
        case DOUBLE:
            return 7;
        case CLASS:
            return 0;
        case ARRAY:
            return 1;
        default:
            error("No array code for type tag " + std::to_string(type->tag));
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
    //TODO pending jump, alive check
    this->emit1(op);
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
    int reg = this->nextreg;
    int w = width(typecode);
    nextreg = reg + w;
    if (nextreg > max_locals) {
        max_locals = nextreg;
    }
    return reg;
}

Code::Code() : nextreg(0), alive(true), cp(0) {
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
    if (adr + 1 >= this->lvar.capacity()) {
        v_size newLen = this->lvar.capacity() << 1;
        if (newLen <= adr) {
            newLen = (v_size) (adr + 10);
        }
        //TODO check where does ele store, stack or heap?
        this->lvar.resize(newLen);
    }
    //TODO if (pendingJumps != null) resolvePending();
    this->lvar.at(adr) = LocalVar::Ptr(new LocalVar(v));
    //TODO state.defined.excl(adr);
}

void Code::emitop2(int op, int od) {

}

void Code::emitop1w(int op, int od1, int od2) {

}

void Code::emitop1w(int op, int od) {

}

void Code::emitInvokestatic(int meth, TypePtr mtype) {

}

void Code::emit1(int od) {
    if (!alive) return;
    if (cp == code.size()) {
        code.reserve(cp == 0 ? 64 : (v_size) cp * 2);
    }
    code[cp++] = (char) od;
}

void Code::endScopes(int start) {
    int preNextReg = this->nextreg;
    this->nextreg = start;
    for (int i = start; i < preNextReg; ++i) {
        this->endScope(i);
    }
}

void Code::endScope(int adr) {
    LocalVar::Ptr sptr = this->lvar.at(adr);
    if (sptr) {
        sptr.reset();
    }
}

void Code::emitNewarray(int elemCode, TypePtr arrType) {
    this->emitop(bytecode::newarray);
    this->emit1(elemCode);
}

void Code::emitInvokespecial(int meth, TypePtr mtype) {

}

void Code::emitInvokevirtual(int meth, TypePtr mtype) {

}

void Code::emitop1(int op, int od) {
    this->emitop(op);
    this->emit1(od);
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
