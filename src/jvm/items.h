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
#include "../tree/tree.h"

class Symtab;

class LocalItem;

class Items;

class Item {
protected:
    Items& items;
public:
    int typecode;

    Item(Items& items, int typecode);

    typedef std::shared_ptr<Item> Ptr;

    virtual Ptr load();

    virtual void store();

    virtual Ptr invoke();

    void duplicate();

    virtual void drop();

    virtual int width();
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

    Item::Ptr load() override;
    void drop() override;

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
    bool nonvirtual;

    MemberItem(Items& items, SymbolPtr member, bool nonvirtual);

    Item::Ptr load() override;

    void store() override;

    Item::Ptr invoke();

    void drop() override;
};

class StaticItem : public Item {
public:
    SymbolPtr member;

    StaticItem(Items& items, SymbolPtr member);

    Item::Ptr load() override;

    void store() override;

    Item::Ptr invoke();
};

class IndexedItem : public Item {
public:
    IndexedItem(Items& items, TypePtr type);
    Item::Ptr load() override;

    void store() override;
    void drop() override;
};

class ImmediateItem : public Item {
public:
    IValueHolder::Ptr value;
    ImmediateItem(Items& items, TypePtr type, IValueHolder::Ptr value);
    Item::Ptr load() override;

    void store() override;
};

class AssignItem : public Item {
public:
    Item::Ptr lhs;
    AssignItem(Items& items, Item::Ptr lhs);
    Item::Ptr load() override;
    void store() override ;
    int width() override ;
    void drop() override ;
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

    Item::Ptr makeMemberItem(SymbolPtr member, bool nonvirtual);

    Item::Ptr makeStaticItem(SymbolPtr member);

    Item::Ptr makeStaticItem(TypePtr type);

    Item::Ptr makeIndexedItem(TypePtr typePtr);

    Item::Ptr makeImmediateItem(TypePtr typePtr, IValueHolder::Ptr value);

    Item::Ptr makeAssignItem(Item::Ptr lhs);

};


#endif //SIMPLECOMPILER_ITEMS_H
