//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_POOL_H
#define SIMPLECOMPILER_POOL_H

#include <memory>
#include <vector>
#include <map>
#include "code/symbols.h"
#include "tree/tree.h"
#include "code/NameAndType.h"

class Wrapper {
public:
    typedef shared_ptr<Wrapper> Ptr;

    template<typename R>
    Wrapper(R value, int type);

    template<typename R>
    R getValue();

    IValueHolder::Ptr value;
    int type;// symbol or type or literal
};

template<typename R>
Wrapper::Wrapper(R value, int type) : value(new ValueHolder<R>(value)), type(type) {

}

template<typename R>
R Wrapper::getValue() {
    ValueHolder<R>* p = static_cast<ValueHolder<R>*>(value.get());
    return p->value;
}

class Pool {
private:

public:
    enum {
        SYMBOL, TYPE, INT, STRING, NAME, NAME_AND_TYPE
    };
    typedef std::shared_ptr<Pool> Ptr;
    std::vector<Wrapper::Ptr> pool;

    Pool();

    void reset();

    int put(SymbolPtr value);

    int put(TypePtr value);

    int put(const Name* value);

    int put(IValueHolder::Ptr value, int typeCode);

    int put(NameAndType::Ptr value);

    int testAndPut(Wrapper::Ptr value);

    int find(Wrapper* value);

    static bool equals(SymbolPtr ptr1, SymbolPtr ptr2);

    static bool equals(MethodSymbolPtr ptr1, MethodSymbolPtr ptr2);

    static bool equals(TypePtr ptr1, TypePtr ptr2);

    static bool equals(MethodTypePtr ptr1, MethodTypePtr ptr2);

    static bool equals(ArrayTypePtr ptr1, ArrayTypePtr ptr2);
};


#endif //SIMPLECOMPILER_POOL_H
