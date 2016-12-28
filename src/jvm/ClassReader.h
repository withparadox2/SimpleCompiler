//
// Created by withparadox2 on 2016/12/25.
//

#ifndef SIMPLECOMPILER_CLASSREADER_H
#define SIMPLECOMPILER_CLASSREADER_H

#include "../code/symbol.h"
class ClassReader {
public:
    static ClassReader &instance();

    //there exists no concept about package.
    ClassSymbol *enterClass(Name &name);
};


#endif //SIMPLECOMPILER_CLASSREADER_H
