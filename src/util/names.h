//
// Created by withparadox2 on 2016/11/12.
//

#ifndef SIMPLECOMPILER_NAMES_H
#define SIMPLECOMPILER_NAMES_H

#include <string>
#include <map>

using std::string;
using std::map;

class Name;

class Names;

class Name {
public:
    Names &names;
    string desc;

    Name(Names &names, const string &desc);

    bool operator==(Name &name);

    bool operator!=(Name &name);
};

class Names {
private:
    map<string, Name*> table;

    Names();

public:
    Name *init;
    Name *_this;
    Name *_class;
    Name *hyphen;

    static Names &instance();

    Name &fromString(const string &str);
};

#endif //SIMPLECOMPILER_NAMES_H
