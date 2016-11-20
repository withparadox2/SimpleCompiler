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
};

class Names {
private:
    map<string, Name*> table;
    static Names inst;

    Names();

public:
    Name *init;
    Name *_this;
    Name *_class;

    static Names &instance();

    Name *fromString(const string &str);

    Name *fromChars(char cs[], int start, int end);
};

#endif //SIMPLECOMPILER_NAMES_H
