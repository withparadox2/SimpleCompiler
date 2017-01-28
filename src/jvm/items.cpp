//
// Created by withparadox2 on 2017/1/27.
//

#include "items.h"
#include "../code/Symtab.h"

Items::Items(Code::Ptr code, Pool::Ptr pool)
        : syms(Symtab::instance()), pool(pool), code(code) {

}

Item::Item(int typecode) : typecode(typecode) {

}

LocalItem::LocalItem(TypePtr type, int reg) : Item(reg), reg(reg){

}
