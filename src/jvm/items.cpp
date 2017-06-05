//
// Created by withparadox2 on 2017/1/27.
//

#include "items.h"
#include "../code/Symtab.h"
#include "../code/type.h"
#include "../util/error.h"

Items::Items(Code::Ptr code, Pool::Ptr pool)
        : syms(Symtab::instance()), pool(pool), code(code) {

    voidItem = Item::Ptr(new Item(*this, bytecode::VOIDcode));
    thisItem = Item::Ptr(new SelfItem(*this, false));
    superItem = Item::Ptr(new SelfItem(*this, true));

    //TODO Exclude void code?
    for (int i = 0; i < bytecode::VOIDcode; i++) {
        stackItem[i] = Item::Ptr(new StackItem(*this, i));
    }

    stackItem[bytecode::VOIDcode] = voidItem;
}

LocalItem::Ptr Items::makeLocalItem(VarSymbolPtr v) {
    return LocalItem::Ptr(new LocalItem(*this, v->type, v->adr));
}

Item::Ptr Items::makeMemberItem(SymbolPtr member, bool nonvirtual) {
    return Item::Ptr(new MemberItem(*this, member, nonvirtual));
}

Item::Ptr Items::makeStaticItem(SymbolPtr member) {
    return Item::Ptr(new StaticItem(*this, member));
}

Item::Ptr Items::makeIndexedItem(TypePtr typePtr) {
    return Item::Ptr(new IndexedItem(*this, typePtr));
}

Item::Ptr Items::makeStackItem(TypePtr type) {
    return stackItem[Code::typecode(type.get())];
}

Item::Ptr Items::makeImmediateItem(TypePtr typePtr, IValueHolder::Ptr value) {
    return Item::Ptr(new ImmediateItem(*this, typePtr, value));
}

Item::Ptr Items::makeAssignItem(Item::Ptr lhs) {
    return Item::Ptr(new AssignItem(*this, lhs));
}

CondItem::Ptr Items::makeCondItem(int opcode) {
    return CondItem::Ptr(new CondItem(*this, opcode, nullptr, nullptr));
}

Item::Item(Items& items, int typecode)
        : items(items), typecode(typecode) {
}

Item::Ptr Item::load() {
    return Item::Ptr();
}

void Item::store() {

}

Item::Ptr Item::invoke() {
    return Item::Ptr();
}

void Item::duplicate() {

}

void Item::drop() {

}

int Item::width() {
    return 0;
}

CondItem::Ptr Item::mkCond() {
    load();
    return items.makeCondItem(bytecode::ifne);
}

LocalItem::LocalItem(Items& items, TypePtr type, int reg)
        : Item(items, Code::typecode(type.get())), reg(reg) {
}

Item::Ptr LocalItem::load() {
    /*
     * Why 3? There are 4 default various opcodes of LOAD
     * for each type. See bytecode.h.
     */
    if (reg <= 3) {
        int op = bytecode::iload_0 + Code::truncate(typecode) * 4 + reg;
        items.code->emitop0(op);
    } else {
        int op = bytecode::iload + Code::truncate(typecode);
        items.code->emitop1w(op, reg);
    }
    return items.stackItem[typecode];
}

void LocalItem::store() {
    if (reg <= 3) {
        int op = bytecode::istore_0 + Code::truncate(typecode) * 4 + reg;
        items.code->emitop0(op);
    } else {
        int op = bytecode::istore + Code::truncate(typecode);
        items.code->emitop1w(op, reg);
    }
    //TODO setDefined
}

StackItem::StackItem(Items& items, int typecode)
        : Item(items, typecode) {
}

Item::Ptr StackItem::load() {
    return Item::Ptr();
}

void StackItem::drop() {
    items.code->emitop0(Code::width(typecode) == 2
                        ? bytecode::pop2
                        : bytecode::pop);
}

SelfItem::SelfItem(Items& items, bool isSuper)
        : Item(items, bytecode::OBJECTcode), isSuper(isSuper) {
}

Item::Ptr SelfItem::load() {
    items.code->emitop0(bytecode::aload_0);
    return items.stackItem[typecode];
}

MemberItem::MemberItem(Items& items, SymbolPtr member, bool nonvirtual)
        : Item(items, Code::typecode(member->type.get())),
          member(member),
          nonvirtual(nonvirtual) {

}

Item::Ptr MemberItem::load() {
    //TODO finish pool
    items.code->emitop2(bytecode::getfield, 0);
    return items.stackItem[typecode];
}

void MemberItem::store() {
    //TODO finish pool
    items.code->emitop2(bytecode::putfield, 0);
}

Item::Ptr MemberItem::invoke() {
    MethodTypePtr mType = dynamic_pointer_cast<MethodType>(member->type);
    int rescode = Code::typecode(mType->restype.get());
    if (this->member->owner->flags & Flags::INTERFACE != 0) {
        //TODO invoke interface method
    } else if (this->nonvirtual) {
        items.code->emitInvokespecial(items.pool->put(this->member), mType);
    } else {
        items.code->emitInvokevirtual(items.pool->put(this->member), mType);
    }
    return items.stackItem[rescode];
}

void MemberItem::drop() {
    //TODO why bytecode::OBJECTcode not typecode?
    items.stackItem[bytecode::OBJECTcode]->drop();
}

StaticItem::StaticItem(Items& items, SymbolPtr member)
        : Item(items, Code::typecode(member->type.get())), member(member) {
}

Item::Ptr StaticItem::load() {
    //TODO finish pool
    items.code->emitop2(bytecode::getstatic, 0);
    return items.stackItem[typecode];
}

void StaticItem::store() {
    //TODO finish pool
    items.code->emitop2(bytecode::putstatic, 0);
}

Item::Ptr StaticItem::invoke() {
    MethodTypePtr mtype = std::dynamic_pointer_cast<MethodType>(member->type);
    int rescode = Code::typecode(mtype->restype.get());
    //TODO finish pool
    items.code->emitInvokestatic(0, mtype);
    return items.stackItem[rescode];
}

IndexedItem::IndexedItem(Items& items, TypePtr type) : Item(items, Code::typecode(type.get())) {}

Item::Ptr IndexedItem::load() {
    items.code->emitop0(bytecode::iaload + typecode);
    return items.stackItem[typecode];
}

void IndexedItem::store() {
    items.code->emitop0(bytecode::iastore + typecode);
}

void IndexedItem::drop() {
    //pop arr[3] equals pop 3 and arr
    items.code->emitop0(bytecode::pop2);
}

ImmediateItem::ImmediateItem(Items& items,
                             TypePtr type,
                             IValueHolder::Ptr value)
        : Item(items, Code::typecode(type.get())), value(value) {
}

Item::Ptr ImmediateItem::load() {
    switch (typecode) {
        case bytecode::INTcode: {
            int val = this->value->getValue<int>();
            log("load Immediate int " + std::to_string(val));
            if (-1 <= val && val <= 5) {
                this->items.code->emitop0(bytecode::iconst_0 + val);
            } else if (-128 <= val && val <= 127) {
                this->items.code->emitop1(bytecode::bipush, val);
            } else if (-32768 <= val && val <= 32767) {
                this->items.code->emitop2(bytecode::sipush, val);
            } else {
                this->ldc();
            }
            break;
        }
        case bytecode::OBJECTcode:
            this->ldc();
            break;
        default:
            error("ImmediateItem::load only support int and string");
    }
    return items.stackItem[typecode];
}

void ImmediateItem::store() {
    Item::store();
}

void ImmediateItem::ldc() {
    int idx = this->items.pool->put(value);
    if (typecode == bytecode::FLOATcode || typecode == bytecode::DOUBLEcode) {
        this->items.code->emitop2(bytecode::ldc2w, idx);
    } else if (idx <= 255) {
        this->items.code->emitop1(bytecode::ldc1, idx);
    } else {
        this->items.code->emitop2(bytecode::ldc2, idx);
    }
}

AssignItem::AssignItem(Items& items, Item::Ptr lhs) : Item(items, lhs->typecode), lhs(lhs) {

}

Item::Ptr AssignItem::load() {
    return Item::load();
}

void AssignItem::store() {
    Item::store();
}

int AssignItem::width() {
    return Item::width();
}

void AssignItem::drop() {
    this->lhs->store();
}


CondItem::CondItem(Items& items, int opcode, Chain* truejumps, Chain* falsejumps)
        : Item(items, bytecode::BYTEcode), opcode(opcode), trueJumps(truejumps), falseJumps(falsejumps) {
}

Item::Ptr CondItem::load() {
}

CondItem::Ptr CondItem::mkCond() {
    return shared_from_this();
}

Chain* CondItem::jumpFalse() {
    return items.code->branch(Code::negate(opcode));

}
