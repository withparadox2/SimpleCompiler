//
// Created by withparadox2 on 2017/1/11.
//
#include "tools.h"

const Name& shortName(const Name& src) {
    size_t pos = src.desc.find_last_of('.');
    if (pos == std::string::npos) {
        return src;
    }

    Name& shortName = Names::instance().fromString(src.desc.substr(pos + 1));
    return shortName;
}