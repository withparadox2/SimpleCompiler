//
// Created by withparadox2 on 2017/1/27.
//

#ifndef SIMPLECOMPILER_CODE_H
#define SIMPLECOMPILER_CODE_H

#include <memory>
#include <vector>
#include "../code/symbols.h"
#include "../code/types.h"
class LocalVar {
public:
    typedef std::shared_ptr<LocalVar> Ptr;
    VarSymbolPtr sym;
    const char16_t reg;
    char16_t start_pc;
    char16_t length;
    LocalVar(VarSymbolPtr v);
    LocalVar* dup();
};

class Code {
private:
    void emitop(int op);
    int newLocal(const Type* type);
    int newLocal(int typecode);

    void addLocalVar(VarSymbolPtr varSymbolPtr);

    std::vector<LocalVar::Ptr> lvar;
public:
    typedef std::shared_ptr<Code> Ptr;

    int nextreg;

    /** The maximum number of local variable slots.*/
    //I guess the slots for local var can be resuable, if not
    //so, why not just keep nextreg instead.
    int max_locals;

    /** The maximum stack size.*/
    int max_stack;

    Code();

    void setDefined(int adr);

    int newLocal(VarSymbolPtr v);

    void emitop0(int op);

    void emitop1w(int op, int od);

    static int typecode(const Type* type);

    static int truncate(int typecode);

    /** The width in bytes of objects of the type.*/
    static int width(int typecode);

    static int width(const Type* type);
};


#endif //SIMPLECOMPILER_CODE_H
