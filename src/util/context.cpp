//
// Created by withparadox2 on 2017/1/25.
//

#include "context.h"

Context& Context::instance() {
    static Context inst;
    return inst;
}

void Context::put(const std::string key, EnableMapHelper* value) {
    keyToObj.insert(std::make_pair(key, std::unique_ptr<EnableMapHelper>(value)));
}

EnableMapHelper::~EnableMapHelper() {

}
