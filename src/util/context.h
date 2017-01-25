//
// Created by ruancongyong on 2017/1/25.
//

#ifndef SIMPLECOMPILER_CONTEXT_H
#define SIMPLECOMPILER_CONTEXT_H

#include <map>
#include <string>
#include <memory>

class EnableMapHelper{
public:
    virtual ~EnableMapHelper();
};

class Context {
private:
    std::map<std::string, std::unique_ptr<EnableMapHelper>> keyToObj;
public:
    static Context& instance();

    template <typename T>
    T* get(const std::string key);

    void put(const std::string key, EnableMapHelper* value);
};

template<typename T>
T* Context::get(const std::string key) {
    if (keyToObj.find(key) == keyToObj.end()) {
        return nullptr;
    }
    EnableMapHelper* base = keyToObj.at(key).get();
    return static_cast<T*>(base);
}

#endif //SIMPLECOMPILER_CONTEXT_H
