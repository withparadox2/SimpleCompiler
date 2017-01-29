//
// Created by withparadox2 on 2017/1/27.
//

#include "items.h"
#include "../code/Symtab.h"

Items::Items(Code::Ptr code, Pool::Ptr pool)
        : syms(Symtab::instance()), pool(pool), code(code) {

    voidItem = Item::Ptr(new Item(*this, bytecode::VOIDcode));
    thisItem = Item::Ptr(new SelfItem(*this, false));
    superItem = Item::Ptr(new SelfItem(*this, true));

    //TODO Exclude void code?
    for(int i = 0; i < bytecode::VOIDcode; i++) {
        stackItem[i] = Item::Ptr(new StackItem(*this, i));
    }

    stackItem[bytecode::VOIDcode] = voidItem;
}

LocalItem::Ptr Items::makeLocalItem(VarSymbolPtr v) {
    return LocalItem::Ptr(new LocalItem(*this, v->type, v->adr));
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

LocalItem::LocalItem(Items& items, TypePtr type, int reg)
        : Item(items, Code::typecode(type)), reg(reg) {
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

}

StackItem::StackItem(Items& items, int typecode)
        : Item(items, typecode) {
}

Item::Ptr StackItem::load() {
    return Item::Ptr();
}

SelfItem::SelfItem(Items& items, bool isSuper)
        : Item(items, bytecode::OBJECTcode), isSuper(isSuper) {
}

Item::Ptr SelfItem::load() {
    items.code->emitop0(bytecode::aload_0);
    return items.stackItem[typecode];
}
