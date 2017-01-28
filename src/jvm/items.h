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
class Item;
class LocalItem;

class Items {
public:
    typedef std::shared_ptr<Items> Ptr;
    Symtab& syms;
    Code::Ptr code;
    Pool::Ptr pool;

    Items(Code::Ptr code, Pool::Ptr pool);
};

class Item {
private:
    int typecode;
public:
    Item(int typecode);
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
    LocalItem(TypePtr type, int reg);

};

#endif //SIMPLECOMPILER_ITEMS_H
