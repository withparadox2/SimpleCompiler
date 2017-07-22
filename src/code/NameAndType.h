//
// Created by withparadox2 on 2017/6/11.
//

#ifndef SIMPLECOMPILER_NAMEANDTYPE_H
#define SIMPLECOMPILER_NAMEANDTYPE_H

#include <memory>
#include "util/names.h"
#include "types.h"

class NameAndType {
public:
    typedef std::shared_ptr<NameAndType> Ptr;
    const Name* name;
    TypePtr type;
    NameAndType(const Name* name, TypePtr type);
};


#endif //SIMPLECOMPILER_NAMEANDTYPE_H
