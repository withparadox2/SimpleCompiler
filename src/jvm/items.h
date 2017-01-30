//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_ITEMS_H
#define SIMPLECOMPILER_ITEMS_H

#include <memory>
#include "Code.h"
#include "Pool.h"
#include "../code/types.h"
#include "bytecode.h"

class Symtab;

class LocalItem;

class Items;

class Item {
protected:
    Items& items;
    int typecode;
public:
    Item(Items& items, int typecode);

    typedef std::shared_ptr<Item> Ptr;

    virtual Ptr load();

    virtual void store();

    virtual Ptr invoke();

    void duplicate();

    void drop();

};

class LocalItem : public Item {
public:
    int reg;
    TypePtr type;

    LocalItem(Items& items, TypePtr type, int reg);

    Item::Ptr load() override;

    void store() override;

};

class StackItem : public Item {
public:
    StackItem(Items& items, int typecode);

    Item::Ptr load();
};

class SelfItem : public Item {
public:
    bool isSuper;

    SelfItem(Items& items, bool isSuper);

    Item::Ptr load() override;
};

class MemberItem : public Item {
public:
    SymbolPtr member;
    bool  nonvirtual;

    MemberItem(Items& items, SymbolPtr member, bool nonvirtual);
    Item::Ptr load() override;
    void store() override;
};

class Items {

public:
    typedef std::shared_ptr<Items> Ptr;

    Item::Ptr voidItem;
    Item::Ptr thisItem;
    Item::Ptr superItem;

    Symtab& syms;
    Code::Ptr code;
    Pool::Ptr pool;
    Item::Ptr stackItem[bytecode::TypeCodeCount];

    Items(Code::Ptr code, Pool::Ptr pool);

    //TODO Can we just use raw pointer?
    LocalItem::Ptr makeLocalItem(VarSymbolPtr v);

};


#endif //SIMPLECOMPILER_ITEMS_H
