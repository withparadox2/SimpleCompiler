//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_CODE_H
#define SIMPLECOMPILER_CODE_H

#include <memory>
#include "../code/symbols.h"
#include "../code/types.h"

class Code {
private:
    void emitop(int op);
public:
    typedef std::shared_ptr<Code> Ptr;
    void setDefined(int adr);

    int newLocal(VarSymbolPtr v);

    void emitop0(int op);

    void emitop1w(int op, int od);

    static int typecode(const TypePtr& type);

    static int truncate(int typecode);

};


#endif //SIMPLECOMPILER_CODE_H
