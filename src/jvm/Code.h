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

    //Ignore these two attributes
    char16_t start_pc;
    char16_t length;

    LocalVar(VarSymbolPtr v);

    LocalVar* dup();
};

class Chain;

class Code {
private:
    bool alive;


    void emitop(int op);

    /** Emit a byte of code.*/
    void emit1(int od);

    void emit2(int od);

    int newLocal(const Type* type);

    int newLocal(int typecode);

    void addLocalVar(VarSymbolPtr varSymbolPtr);

    std::vector<LocalVar::Ptr> lvar;

    void endScope(int adr);
public:
    /** the current code pointer.*/
    int cp;

    typedef std::shared_ptr<Code> Ptr;

    std::vector<char> code;
    typedef std::vector<char>::size_type v_size;

    int nextreg;

    /** The maximum number of local variable slots.*/
    //I guess the slots for local var can be resuable, if not
    //so, why not just keep nextreg instead.
    int max_locals;

    /** The maximum stack size.*/
    int max_stack;

    Chain* pendingJumps;

    Code();

    void endScopes(int start);

    void setDefined(int adr);

    int newLocal(VarSymbolPtr v);

    void emitop0(int op);

    void emitop1(int op, int od);

    void emitop1w(int op, int od);

    /** Emit an opcode with two one-byte operand fields;
     *  widen if either field does not fit in a byte.
     */
    void emitop1w(int op, int od1, int od2);

    void emitop2(int op, int od);

    int emitJump(int opcode);

    void emitInvokestatic(int meth, TypePtr mtype);
    void emitInvokespecial(int meth, TypePtr mtype);
    void emitInvokevirtual(int meth, TypePtr mtype);

    void emitNewarray(int elemCode, TypePtr arrType);

    Chain* branch(int opcode);

    void resolve(Chain* chain);

    void resolve(Chain* chain, int target);

    void put2(int pc, int od);
    void put1(int pc, int od);

    int get1(int pc);

    void printCode();

    static Chain* mergeChains(Chain* c1, Chain* c2);

    static int negate(int opcode);

    static int typecode(const Type* type);

    static int arraycode(const Type* type);

    static int truncate(int typecode);

    /** The width in bytes of objects of the type.*/
    static int width(int typecode);

    static int width(const Type* type);

    static int width(TypeList types);
};

class Chain {
public:
    const int pc;
    Chain* next;//TODO use sharedptr
    Chain(int pc, Chain* next);
};


#endif //SIMPLECOMPILER_CODE_H
