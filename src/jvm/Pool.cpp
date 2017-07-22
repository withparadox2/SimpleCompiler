//
// Created by withparadox2 on 2017/1/27.
//

#include "Pool.h"
#include "code/symbol.h"
#include "code/type.h"
#include "jvm/bytecode.h"

Pool::Pool() {

}

int Pool::put(SymbolPtr value) {
    std::string desc = value->name.desc;
    Wrapper::Ptr src(new Wrapper(value, SYMBOL));
    return testAndPut(src);
}

void Pool::reset() {
    pool.clear();
    pool.push_back(Wrapper::Ptr(new Wrapper(Names::instance().STUB_FOR_COMPILE, NAME)));
}


int Pool::put(IValueHolder::Ptr value, int typeCode) {
    Wrapper* w;
    if (typeCode == bytecode::INTcode) {
        w = new Wrapper(value->getValue<int>(), INT);
    } else {
        w = new Wrapper(value->getValue<std::string>(), STRING);
    }
    Wrapper::Ptr src(w);
    return testAndPut(src);
}

int Pool::put(TypePtr value) {
    Wrapper::Ptr src(new Wrapper(value, TYPE));
    return testAndPut(src);
}

int Pool::put(const Name* name) {
    log("put to pool, name = " + name->desc);
    Wrapper::Ptr src(new Wrapper(name, NAME));
    return testAndPut(src);
}


int Pool::put(NameAndType::Ptr value) {
    Wrapper::Ptr src(new Wrapper(value, NAME_AND_TYPE));
    return testAndPut(src);
}

int Pool::testAndPut(Wrapper::Ptr src) {
    int index = find(src.get());
    if (index < 0) {
        pool.push_back(src);
        index = (int) (pool.size() - 1);
    }
    return index;
}


int Pool::find(Wrapper* value) {
    int index = 0;
    for (auto iter = pool.begin(); iter != pool.end(); iter++, index++) {
        if (iter->get()->type != value->type) {
            continue;
        }
        switch (value->type) {
            case SYMBOL: {
                SymbolPtr ptr = value->getValue<SymbolPtr>();
                SymbolPtr ptrInPool = iter->get()->getValue<SymbolPtr>();
                if (equals(ptr, ptrInPool)) {
                    return index;
                }
                break;
            }
            case TYPE: {
                TypePtr ptr = value->getValue<TypePtr>();
                TypePtr ptrInPool = iter->get()->getValue<TypePtr>();

                if (ptrInPool->tag == ptr->tag
                    && ptrInPool->tsym.lock()->name == ptr->tsym.lock()->name) {
                    return index;
                }
                break;
            }
            case INT: {
                if (value->getValue<int>() == iter->get()->getValue<int>()) {
                    return index;
                }
                break;
            }
            case STRING: {
                if (value->getValue<std::string>() == iter->get()->getValue<std::string>()) {
                    return index;
                }
                break;
            }
            case NAME: {
                if (value->getValue<Name*>() == iter->get()->getValue<Name*>()) {
                    return index;
                }
                break;
            }
            case NAME_AND_TYPE: {
                NameAndType::Ptr p1 = value->getValue<NameAndType::Ptr>();
                NameAndType::Ptr p2 = iter->get()->getValue<NameAndType::Ptr>();
                if (equals(p1->type, p2->type) && *p1->name == *p2->name) {
                    return index;
                }
                break;
            }
        }
    }
    return -1;
}

bool Pool::equals(SymbolPtr ptr1, SymbolPtr ptr2) {
    if (ptr1->kind != ptr2->kind) {
        return false;
    }
    if (ptr1->name != ptr2->name) {
        return false;
    }
    if (ptr1->kind == Kind::MTH) {
        return equals(dynamic_pointer_cast<MethodSymbol>(ptr1),
                      dynamic_pointer_cast<MethodSymbol>(ptr2));
    }
    return true;
}

bool Pool::equals(TypePtr ptr1, TypePtr ptr2) {
    if (ptr1->tag != ptr2->tag) {
        return false;
    }
    if (ptr1->tag == TypeTags::METHOD) {
        return equals(dynamic_pointer_cast<MethodType>(ptr1),
                      dynamic_pointer_cast<MethodType>(ptr2));
    } else if (ptr1->tag == TypeTags::ARRAY) {
        return equals(dynamic_pointer_cast<ArrayType>(ptr1),
                      dynamic_pointer_cast<ArrayType>(ptr2));
    }
    return true;
}

bool Pool::equals(MethodSymbolPtr ptr1, MethodSymbolPtr ptr2) {
    return equals(ptr1->owner, ptr2->owner) && equals(ptr1->type, ptr2->type);
}

bool Pool::equals(MethodTypePtr ptr1, MethodTypePtr ptr2) {
    if (ptr1->argtypes.size() != ptr2->argtypes.size()) {
        return false;
    }
    auto aIter = ptr1->argtypes.begin();
    auto pIter = ptr2->argtypes.begin();
    while (aIter != ptr1->argtypes.end() && pIter != ptr2->argtypes.end()) {
        if (!equals(*aIter, *pIter)) {
            return false;
        }
        aIter++;
        pIter++;
    }

    return equals(ptr1->restype, ptr2->restype);
}

bool Pool::equals(ArrayTypePtr ptr1, ArrayTypePtr ptr2) {
    return equals(ptr1->elemtype, ptr2->elemtype);
}

