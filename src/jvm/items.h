//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_ITEMS_H
#define SIMPLECOMPILER_ITEMS_H

#include <memory>
#include "Code.h"
#include "Pool.h"
#include "../code/types.h"

class Symtab;
class LocalItem;
class Items;

class Item {
private:
    int typecode;
protected:
    Items& items;
public:
    Item(Items& items, int typecode);
    typedef std::shared_ptr<Item> Ptr;
    virtual Ptr load() = 0;
    virtual void store() = 0;
    virtual Ptr invoke() = 0;
    void duplicate();
    void drop();

};

class LocalItem : public Item {
public:
    int reg;
    TypePtr type;
    LocalItem(Items& items, TypePtr type, int reg);
    Item::Ptr load();
    void store();

};

class Items {
public:
    typedef std::shared_ptr<Items> Ptr;
    Symtab& syms;
    Code::Ptr code;
    Pool::Ptr pool;

    Items(Code::Ptr code, Pool::Ptr pool);

    //TODO can we just use raw pointer?
    LocalItem::Ptr makeLocalItem(VarSymbolPtr v);

    /**Emit an opcode with no operand field.*/
    void emitop0(int op);
};


#endif //SIMPLECOMPILER_ITEMS_H
