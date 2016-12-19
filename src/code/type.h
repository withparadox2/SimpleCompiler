//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_TYPE_H
#define SIMPLECOMPILER_TYPE_H

class TypeSymbol;

class Type {
public:
    int tag;
    TypeSymbol *tsym;
};


#endif //SIMPLECOMPILER_TYPE_H
