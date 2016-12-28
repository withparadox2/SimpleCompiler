//
// Created by withparadox2 on 2016/12/25.
//

#include "ClassReader.h"


ClassReader &instance() {
    static ClassReader reader;
    return reader;
}