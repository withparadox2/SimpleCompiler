//
// Created by withparadox2 on 2017/1/11.
//

#ifndef SIMPLECOMPILER_COMPLETER_H
#define SIMPLECOMPILER_COMPLETER_H

#include <memory>
class Symbol;
class Completer {
public:
    virtual void complete(std::shared_ptr<Symbol> sym) = 0;
};
#endif //SIMPLECOMPILER_COMPLETER_H
