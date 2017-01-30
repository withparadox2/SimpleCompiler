//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_POOL_H
#define SIMPLECOMPILER_POOL_H

#include <memory>
#include <vector>
#include <map>
#include "../code/symbols.h"

class Pool {
private:
public:
    typedef std::shared_ptr<Pool> Ptr;
    static const int MAX_ENTRIES = 0xFFFF;
    static const int MAX_STRING_LENGTH = 0xFFFF;

    /** Index of next constant to be entered.
     */
    int pp;
    Pool();
    void reset();
    int put(SymbolPtr value);

};


#endif //SIMPLECOMPILER_POOL_H
