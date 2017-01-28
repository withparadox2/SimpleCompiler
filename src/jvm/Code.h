//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_CODE_H
#define SIMPLECOMPILER_CODE_H

#include <memory>
#include "../code/symbols.h"
#include "../code/types.h"

class Code {
public:
    typedef std::shared_ptr<Code> Ptr;
    void setDefined(int adr);

    int newLocal(VarSymbolPtr v);

    static int typecode(const Type& type);

};


#endif //SIMPLECOMPILER_CODE_H
