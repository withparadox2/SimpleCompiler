//
// Created by withparadox2 on 2017/1/24.
//

#ifndef SIMPLECOMPILER_CLASSWRITER_H
#define SIMPLECOMPILER_CLASSWRITER_H

#include <fstream>
#include "code/symbol.h"
#include "jvm/Pool.h"
#include "Code.h"
#include "code/NameAndType.h"
#include "tree/tree.h"

#define DEBUG true


class ClassWriter {
private:
    Names* names;
    Pool::Ptr pool;
    ofstream ofs;

    void writeMethod(MethodSymbolPtr sym);

    Name& typeSig(TypePtr type);

    std::string assembleSig(TypePtr type);

    std::string assembleSig(TypeList type);

    string assembleClassSig(TypePtr shared_ptr);

    string dotToSlash(const string& from);

    int beginAttrs();

    int writeAttr(Name* pName);

    void writeCode(Code::Ptr code);

    void writeStackMap(Code::Ptr code);

    void endAttr(int idx);

    void endAttrs(int idx, int acount);

    void writePool(Pool::Ptr pool);

    void wirteFileAndClose(std::vector<char>& buf);

    NameAndType::Ptr nameAndType(SymbolPtr sym);

public:
    ClassWriter();

    std::vector<char> poolbuf;
    std::vector<char> databuf;

    void append1(std::vector<char>& buf, int val);

    void append2(std::vector<char>& buf, int val);

    void append4(std::vector<char>& buf, int val);

    void put4(std::vector<char>& buf, int start, int val);

    void put2(std::vector<char>& buf, int start, int val);

    void appendBytes(std::vector<char>& buf, int len, std::vector<char>& src);

    void appendStr(std::vector<char>& buf, int len, const char* str);

    void writeClass(JCClassDecl* cTree, std::string path);
};


#endif //SIMPLECOMPILER_CLASSWRITER_H
