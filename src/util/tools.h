//
// Created by withparadox2 on 2017/1/11.
//

#ifndef SIMPLECOMPILER_TOOLS_H
#define SIMPLECOMPILER_TOOLS_H

#include <vector>
#include "stdarg.h"
#include "names.h"

const Name& shortName(const Name& src);

template<typename T>
std::vector<T> ofList(T e1) {
    std::vector<T> vec;
    vec.push_back(e1);
    return vec;
}

template<typename T>
std::vector<T> ofList(T e1, T e2) {
    std::vector<T> vec;
    vec.push_back(e1);
    vec.push_back(e2);
    return vec;
}

#endif //SIMPLECOMPILER_TOOLS_H
