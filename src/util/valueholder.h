//
// Created by withparadox2 on 2017/7/24.
//

#ifndef SIMPLECOMPILER_VALUEHOLDER_H
#define SIMPLECOMPILER_VALUEHOLDER_H

#include <memory>

class IValueHolder {
public:
    typedef std::shared_ptr<IValueHolder> Ptr;

    template<typename R>
    R getValue();
};

template<typename T>
class ValueHolder : public IValueHolder {
public://let outer class read value
    T value;

    ValueHolder(T value);
};

template<typename R>
R IValueHolder::getValue() {
    ValueHolder<R>* p = static_cast<ValueHolder<R>*>(this);
    return p->value;
}

template<typename T>
ValueHolder<T>::ValueHolder(T value) : value(value) {}

#endif //SIMPLECOMPILER_VALUEHOLDER_H
