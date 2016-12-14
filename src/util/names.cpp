//
// Created by withparadox2 on 2016/11/12.
//
#include "names.h"

using std::make_pair;

Name::Name(Names &names, const string &desc) : names(names), desc(desc) {}

bool Name::operator==(Name &name) {
    return desc.compare(name.desc) == 0;
}

bool Name::operator!=(Name &name) {
    return desc.compare(name.desc) != 0;;
}

Names::Names() {
    init = fromString("<init>");
    _this = fromString("this");
    _class = fromString("class");
    hyphen = fromString("-");
}

Name *Names::fromString(const string &str) {
    if (table.find(str) == table.end()) {
        Name *name = new Name(*this, str);
        table.insert(make_pair(str, name));
        return name;
    }
    return table[str];
}

Names &Names::instance() {
    return inst;
}

Names Names::inst;

