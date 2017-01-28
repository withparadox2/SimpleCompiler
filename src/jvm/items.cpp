//
// Created by withparadox2 on 2017/1/27.
//

#include "items.h"
#include "../code/Symtab.h"

Items::Items(Code::Ptr code, Pool::Ptr pool)
        : syms(Symtab::instance()), pool(pool), code(code) {
}

LocalItem::Ptr Items::makeLocalItem(VarSymbolPtr v) {
    return LocalItem::Ptr(new LocalItem(*this, v->type, v->adr));
}

void Items::emitop0(int op) {

}

Item::Item(Items& items, int typecode) : items(items), typecode(typecode) {
}

LocalItem::LocalItem(Items& items, TypePtr type, int reg)
        : Item(items, Code::typecode(type)), reg(reg) {
}

Item::Ptr LocalItem::load() {
    /*
     * Why 3? There are 4 variant bytecode of load
     * for each type.
     */
    if (reg <= 3) {
        items.code
    } else {

    }
}

void LocalItem::store() {

}


