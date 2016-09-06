//
// Created by ruancongyong on 2016/9/6.
//

#ifndef SIMPLECOMPILER_UTILS_H
#define SIMPLECOMPILER_UTILS_H

#include <vector>

template<class T>
void releaseVec(std::vector<T> vec) {
    for (auto iter = vec.begin(); iter != vec.end(); iter++) {
        delete *iter;
    }
}

#endif //SIMPLECOMPILER_UTILS_H
