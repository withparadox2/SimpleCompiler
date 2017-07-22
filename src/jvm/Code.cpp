//
// Created by withparadox2 on 2017/1/27.
//

#include "Code.h"
#include "code/type.h"
#include "bytecode.h"
#include "util/error.h"
#include "code/stackframe.h"

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
    emitop(op);
}

void Code::emitop(int op) {
    if (pendingJumps != nullptr) {
        resolvePending();
    }
    if (pendingStackMap) {
        pendingStackMap = false;
        emitStackMapFrame(cp, getLocalSize());
    }
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

Code::Code(SymbolPtr meth) : nextreg(0), alive(true), cp(0), meth(meth), pendingJumps(nullptr) {
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
    if (pendingJumps != nullptr) {
        resolvePending();
    }
    this->lvar.at(adr) = LocalVar::Ptr(new LocalVar(v));
    //TODO state.defined.excl(adr);
}

void Code::emitop2(int op, int od) {
    emitop(op);
    if (!alive) return;
    emit2(od);
}

void Code::emitop1w(int op, int od1, int od2) {

}

void Code::emitop1w(int op, int od) {
    if (od > 0xFF) {
        emitop(bytecode::wide);
        emitop(op);
        emit2(od);
    } else {
        emitop(op);
        emit1(od);
    }
}

void Code::emitInvokestatic(int meth, TypePtr mtype) {
    int argsize = width(mtype->getParameterTypes());
    emitop(bytecode::invokestatic);
    if (!alive) return;
    emit2(meth);
}

void Code::emit1(int od) {
    if (!alive) return;
    if (cp == code.size()) {
        code.resize(cp == 0 ? 64 : (v_size) cp * 2);
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
    LocalVar::Ptr& sptr = this->lvar.at(adr);
    if (sptr) {
        sptr.reset();
    }
}

void Code::emitNewarray(int elemCode, TypePtr arrType) {
    this->emitop(bytecode::newarray);
    this->emit1(elemCode);
}

void Code::emitInvokespecial(int meth, TypePtr mtype) {
    int argsize = width(mtype->getParameterTypes());
    emitop(bytecode::invokespecial);
    if (!alive) return;
    emit2(meth);
}

void Code::emitInvokevirtual(int meth, TypePtr mtype) {
    int argsize = this->width(mtype->getParameterTypes());
    this->emitop0(bytecode::invokevirtual);

    if (!alive) return;

    this->emit2(meth);
}

void Code::emitop1(int op, int od) {
    this->emitop(op);
    this->emit1(od);
}

int Code::width(TypeList types) {
    int w = 0;
    for (auto iter = types.begin(); iter != types.end(); iter++) {
        w = w + width(iter->get());
    }
    return w;
}

void Code::emit2(int od) {
    if (!alive) return;
    if (this->cp + 2 > code.size()) {
        this->emit1(od >> 8);
        this->emit1(od);
    } else {
        code[cp++] = (char) (od >> 8);
        code[cp++] = (char) od;
    }
}

int Code::negate(int opcode) {
    if (opcode == bytecode::if_acmp_null) return bytecode::if_acmp_nonnull;
    else if (opcode == bytecode::if_acmp_nonnull) return bytecode::if_acmp_null;
    else return ((opcode + 1) ^ 1) - 1;
}

Chain* Code::branch(int opcode) {
    Chain* result = nullptr;
    if (opcode == bytecode::goto_) {
        result = pendingJumps;
        pendingJumps = nullptr;
    }
    if (opcode != bytecode::jsr) {
        result = new Chain(emitJump(opcode), result);
        if (opcode == bytecode::goto_) {
//            alive = false;
        }
    }
    return result;
}

int Code::emitJump(int opcode) {
    emitop2(opcode, 0);
    return cp - 3;
}

void Code::resolve(Chain* chain) {
    pendingJumps = mergeChains(chain, pendingJumps);
}

Chain* Code::mergeChains(Chain* chain1, Chain* chain2) {
    if (chain2 == nullptr) return chain1;
    if (chain1 == nullptr) return chain2;
    if (chain1->pc < chain2->pc)
        return new Chain(
                chain2->pc,
                mergeChains(chain1, chain2->next));
    return new Chain(
            chain1->pc,
            mergeChains(chain1->next, chain2));
}

void Code::resolve(Chain* chain, int target) {

    for (; chain != nullptr; chain = chain->next) {
        //Jump can be dropped
        if (get1(chain->pc) == bytecode::goto_
            && chain->pc + 3 == target
            && target == cp) {
            cp = cp - 3;
            target = target - 3;
        }
        put2(chain->pc + 1, target - chain->pc);
        if (cp == target) {
           pendingStackMap = true;
        }
    }

}

void Code::put2(int pc, int od) {
    put1(pc, od >> 8);
    put1(pc + 1, od);
}

void Code::put1(int pc, int od) {
    code[pc] = (char) od;
}

int Code::get1(int pc) {
    return code[pc] & 0xff;
}

void Code::printCode() {
    std::cout << "length = " << code.size() << std::endl;
    for (auto i = code.begin(); i != code.end(); i++) {
        std::cout << (int) (uint8_t) *i << " ";
    }
    std::cout << std::endl;
}

int Code::entryPoint() {
    pendingStackMap = true;
    alive = true;
    return this->cp;
}

void Code::emitStackMapFrame(int pc, int localSize) {
    if (!lastFrame) {
        lastFrame = getInitialFrame();
    }

    std::vector<LocalVar::Ptr> localList;
    for (int i = 0; i < localSize; ++i) {
        localList.push_back(lvar.at(i));
    }

    StackMapFrame::Ptr frame = std::make_shared<StackMapFrame>();
    frame->pc = pc;

    for (auto iter = localList.begin(); iter != localList.end(); iter++) {
        frame->locals.push_back(iter->get()->sym->type);
    }

    stackMapTableBuffer.push_back(
            buildStackFrame(frame,
                            lastFrame->pc,
                            lastFrame->locals));
    lastFrame = frame;
}

StackMapFrame::Ptr Code::getInitialFrame() {
    StackMapFrame* frame = new StackMapFrame;

    //Non static method has `this` as its first parameter
    if (!meth->isStatic()) {
        TypePtr thisType = meth->owner->type;
        if (meth->isConstructor()) {
            //TODO wrap thisType into uninitializedThis
            frame->locals.push_back(thisType);
        } else {
            frame->locals.push_back(thisType);
        }
    }

    TypeList argTypes = dynamic_pointer_cast<MethodType>(meth->type)->argtypes;
    for (auto iter = argTypes.begin(); iter != argTypes.end(); iter++) {
        frame->locals.push_back(*iter);
    }

    frame->pc = -1;
    return StackMapFrame::Ptr(frame);
}

StackMapTableFrame::Ptr
Code::buildStackFrame(StackMapFrame::Ptr thisFrame, int prevPc, TypeList prevLocals) {
    TypeList& locals = thisFrame->locals;
    int offsetDelta = thisFrame->pc - prevPc - 1;
    //TODO consider stack count
    int diffLen = compare(prevLocals, locals);
    if (diffLen == 0) {
        return std::make_shared<SameFrame>(offsetDelta);
    } else if (-4 < diffLen && diffLen < 0) {
        TypeList newList;
        for (std::size_t i = prevLocals.size(); i < locals.size(); ++i) {
            newList.push_back(locals[i]);
        }
        return std::make_shared<AppendFrame>(SAME_FRAME_EXTENDED - diffLen, offsetDelta, newList);
    } else if (0 < diffLen && diffLen < 4) {
        return std::make_shared<ChopFrame>(SAME_FRAME_EXTENDED - diffLen, offsetDelta);
    }
    error("[buildStackFrame] not support any way.");
}

int Code::compare(TypeList& list1, TypeList& list2) {
    int diffLen = (int) (list1.size() - list2.size());
    if (diffLen > 4 || diffLen < -4) {
        return MAX_VALUE;
    }
    int len = (int) (diffLen > 0 ? list2.size() : list1.size());
    for (int i = 0; i < len; ++i) {
        if (!Pool::equals(list1.at(i), list2.at(i))) {
            return MAX_VALUE;
        }
    }
    return diffLen;
}

int Code::getLocalSize() {
    int size = 0;
    for (auto iter = lvar.begin(); iter != lvar.end(); iter++) {
        if (*iter) {
            size++;
        }
    }
    return size;
}

void Code::resolvePending() {
    Chain* c = pendingJumps;
    pendingJumps = nullptr;
    resolve(c, cp);
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

Chain::Chain(int pc, Chain* next) : pc(pc), next(next) {
}
