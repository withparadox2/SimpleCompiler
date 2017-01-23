//
// Created by withparadox2 on 2016/8/31.
//

#include "error.h"

void error(std::string desc) {
    std::cerr << desc << std::endl;
    exit(-1);
}