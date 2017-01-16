//
// Created by withparadox2 on 2016/11/12.
//
#include "names.h"

using std::make_pair;

Name::Name(Names& names, const string& desc) : names(names), desc(desc) {}

bool Name::operator==(const Name& name) const {
    return desc.compare(name.desc) == 0;
}

bool Name::operator!=(const Name& name) const {
    return desc.compare(name.desc) != 0;
}

bool Name::operator!=(const Name& name) {
    return static_cast<const Name&>(*this) != name;
}

bool Name::operator==(const Name& name) {
    return static_cast<const Name&>(*this) == name;
}

Names::Names() {
    init = &fromString("<init>");
    _this = &fromString("this");
    _class = &fromString("class");
    _super = &fromString("super");
    hyphen = &fromString("-");
    Array = &fromString("Array");
    METHOD = &fromString("METHOD");
}

Name& Names::fromString(const string& str) {
    if (table.find(str) == table.end()) {
        std::unique_ptr<Name> name(new Name(*this, str));
        table.insert(make_pair(str, std::move(name)));
    }
    return *table[str];
}

Names& Names::instance() {
    static Names inst;
    return inst;
}


