//
// Created by withparadox2 on 2016/11/12.
//

#ifndef SIMPLECOMPILER_NAMES_H
#define SIMPLECOMPILER_NAMES_H

#include <string>
#include <map>
#include <memory>
#include "context.h"

using std::string;
using std::map;

class Name;

class Names;

class Name {
public:
    Names& names;
    string desc;

    Name(Names& names, const string& desc);

    bool operator==(const Name& name) const;

    bool operator!=(const Name& name) const;

    bool operator!=(const Name& name);

    bool operator==(const Name& name);
};

class Names : public EnableMapHelper {
private:
    map<string, std::unique_ptr<Name>> table;

    Names();

public:
    Name* init;
    Name* _this;
    Name* _class;
    Name* _super;
    Name* hyphen;
    Name* Array;
    Name* METHOD;
    Name* empty;

    static Names& instance();

    Name& fromString(const string& str);
};

#endif //SIMPLECOMPILER_NAMES_H
