//
// Created by withparadox2 on 2017/1/24.
//

#include "ClassWriter.h"
#include "../code/Flags.h"
#include "../code/type.h"
#include "../code/scope.h"
#include "../util/error.h"
#include "../jvm/ClassFile.h"


void ClassWriter::writeClass(JCClassDecl* cTree, std::string path) {
    ofs.open(path, std::ofstream::binary | std::ofstream::trunc);
    if (!ofs.is_open()) {
        error("can not open output stream : " + path);
    }

    ClassSymbolPtr cPtr = cTree->sym;
    ClassSymbol& c = *cPtr.get();
    pool = c.pool;
    databuf.resize(0);
    poolbuf.resize(0);

    if (DEBUG) {
        log("Start generate class file, pay attention!");
    }

    TypePtr superType = dynamic_pointer_cast<ClassType>(cPtr->type)->supertype_field;

    int flags = (int) c.flags;
    if (flags & Flags::INTERFACE) {
        flags |= Flags::ACC_SUPER;
    }
    append2(databuf, flags);
    append2(databuf, pool->put(cPtr));
    append2(databuf, pool->put(superType->tsym.lock()));
    append2(databuf, 0);//interface length

    int fieldsCount = 0;
    int methodsCount = 0;

    for (auto iter = cTree->defs.begin();
         iter != cTree->defs.end();
         iter++) {
        int treeTag = iter->get()->treeTag;
        if (treeTag == Tree::METHODDEF) {
            methodsCount++;
        } else if (treeTag == Tree::VARDEF) {
            fieldsCount++;
        }
    }
    append2(databuf, 0);//TODO support fields
    append2(databuf, methodsCount);

    for (auto iter = cTree->defs.begin();
         iter != cTree->defs.end();
         iter++) {
        int treeTag = iter->get()->treeTag;
        if (treeTag == Tree::METHODDEF) {
            writeMethod(dynamic_cast<JCMethodDecl*>(iter->get())->sym);
        }
    }

    append2(databuf, 0);//no extra attributes

    //start pool
    append4(poolbuf, 0xCAFEBABE);
    append2(poolbuf, 0);
    append2(poolbuf, 51);

    writePool(c.pool);
    appendBytes(poolbuf, (int) databuf.size(), databuf);

    wirteFileAndClose(poolbuf);
}


void ClassWriter::append1(std::vector<char>& buf, int val) {
    buf.push_back((char) val);
}

void ClassWriter::append2(std::vector<char>& buf, int val) {
    buf.push_back((char) ((val >> 8) & 0xFF));
    buf.push_back((char) (val & 0xFF));
}


void ClassWriter::append4(std::vector<char>& buf, int val) {
    buf.push_back((char) ((val >> 24) & 0xFF));
    buf.push_back((char) ((val >> 16) & 0xFF));
    buf.push_back((char) ((val >> 8) & 0xFF));
    buf.push_back((char) (val & 0xFF));
}


void ClassWriter::put4(std::vector<char>& buf, int start, int val) {
    buf[start] = ((char) ((val >> 24) & 0xFF));
    buf[start + 1] = ((char) ((val >> 16) & 0xFF));
    buf[start + 2] = ((char) ((val >> 8) & 0xFF));
    buf[start + 3] = ((char) (val & 0xFF));
}


void ClassWriter::put2(std::vector<char>& buf, int start, int val) {
    buf[start] = ((char) ((val >> 8) & 0xFF));
    buf[start + 1] = ((char) (val & 0xFF));
}

void ClassWriter::writeMethod(MethodSymbolPtr sym) {
    append2(databuf, (int) sym->flags);
    append2(databuf, pool->put(&sym->name));
    append2(databuf, pool->put(&typeSig(sym->type)));

    int acountIdx = beginAttrs();
    int acount = 0;

    if (sym->code) {
        int alenIdx = writeAttr(names->Code);
        writeCode(sym->code);
        sym->code = nullptr; // to conserve space
        endAttr(alenIdx);
        acount++;
    }

    endAttrs(acountIdx, acount);
}

Name& ClassWriter::typeSig(TypePtr type) {
    std::string sig = assembleSig(type);
    return names->fromString(sig);
}

std::string ClassWriter::assembleSig(TypePtr type) {
    std::string result = "";
    switch (type->tag) {
        case TypeTags::INT: {
            result += "I";
            break;
        }
        case TypeTags::BOOLEAN: {
            result += "Z";
            break;
        }
        case TypeTags::VOID: {
            result += "V";
            break;
        }
        case TypeTags::CLASS: {
            result += "L";
            result += assembleClassSig(type);
            result += ";";
            break;
        }
        case TypeTags::ARRAY: {
            ArrayType* at = dynamic_cast<ArrayType*>(type.get());
            result += "[";
            result += assembleSig(at->elemtype);
            break;
        }
        case TypeTags::METHOD: {
            MethodType* mt = dynamic_cast<MethodType*>(type.get());
            result += "(";
            result += assembleSig(mt->argtypes);
            result += ")";
            result += assembleSig(mt->restype);
            break;
        }
        default:
            error("type not support in assembleSig : " + std::to_string(type->tag));
            break;
    }
    return result;
}

std::string ClassWriter::assembleSig(TypeList list) {
    std::string result;
    for (auto iter = list.begin(); iter != list.end(); iter++) {
        result += assembleSig(*iter);
    }
    return result;
}

string ClassWriter::assembleClassSig(TypePtr ptr) {
    ClassSymbolPtr c = dynamic_pointer_cast<ClassSymbol>(ptr->tsym.lock());
    return dotToSlash(c->flatName->desc);;
}

int ClassWriter::beginAttrs() {
    append2(databuf, 0);
    return (int) databuf.size();
}


void ClassWriter::endAttrs(int idx, int acount) {
    put2(databuf, idx - 2, acount);
}

ClassWriter::ClassWriter() {
    names = &Names::instance();
}

int ClassWriter::writeAttr(Name* pName) {
    append2(databuf, pool->put(pName));
    append4(databuf, 0);//will be rewritten in endAttr(int)
    return (int) databuf.size();
}

void ClassWriter::endAttr(int idx) {
    put4(databuf, idx - 4, (int) (databuf.size() - idx));
}

void ClassWriter::writeCode(Code::Ptr code) {
    append2(databuf, 10);//TODO max_stack
    append2(databuf, code->max_locals);
    append4(databuf, code->cp);
    appendBytes(databuf, code->cp, code->code);
    append2(databuf, 0);
    append2(databuf, 0);
}

void ClassWriter::appendBytes(std::vector<char>& buf, int len, std::vector<char>& src) {
    for (int i = 0; i < len; ++i) {
        buf.push_back(src[i]);
    }
}

void ClassWriter::writePool(Pool::Ptr poolPtr) {
    int poolCountIdx = (int) poolbuf.size();
    append2(poolbuf, 0);
    int i = 1;
    auto& pool = poolPtr->pool;
    while (i < pool.size()) {
        Wrapper::Ptr w = pool.at(i);
        switch (w->type) {
            case Pool::SYMBOL: {
                SymbolPtr symPtr = w->getValue<SymbolPtr>();
                if (symPtr->kind == Kind::MTH) {
                    MethodSymbolPtr mPtr = dynamic_pointer_cast<MethodSymbol>(symPtr);
                    append1(poolbuf,
                            (mPtr->owner->flags & Flags::INTERFACE) != 0
                            ? ClassFile::CONSTANT_InterfaceMethodref
                            : ClassFile::CONSTANT_Methodref);
                    append2(poolbuf, poolPtr->put(mPtr->owner));
                    append2(poolbuf, poolPtr->put(nameAndType(mPtr)));
                } else if (symPtr->kind == Kind::TYP) {
                    ClassSymbolPtr cPtr = dynamic_pointer_cast<ClassSymbol>(symPtr);
                    append1(poolbuf, ClassFile::CONSTANT_Class);
                    if (cPtr->type->tag == TypeTags::ARRAY) {
                        append2(poolbuf, poolPtr->put(&typeSig(cPtr->type)));
                    } else {
                        append2(poolbuf, poolPtr->put(&names->fromString(dotToSlash(cPtr->flatName->desc))));
                    }
                } else if (symPtr->kind == Kind::VAR) {
                    append1(poolbuf, ClassFile::CONSTANT_Fieldref);
                    append2(poolbuf, poolPtr->put(symPtr->owner));
                    append2(poolbuf, poolPtr->put(nameAndType(symPtr)));
                } else {
                    error("Not handle in writePool, symbol name = " + symPtr->name.desc);
                }
                break;
            }
            case Pool::NAME: {
                append1(poolbuf, ClassFile::CONSTANT_Utf8);
                Name* value = w->getValue<Name*>();
                append2(poolbuf, (int) value->desc.size());
                appendStr(poolbuf, (int) value->desc.size(), value->desc.c_str());
                break;
            }
            case Pool::NAME_AND_TYPE: {
                append1(poolbuf, ClassFile::CONSTANT_NameandType);
                NameAndType::Ptr value = w->getValue<NameAndType::Ptr>();
                append2(poolbuf, poolPtr->put(value->name));
                append2(poolbuf, poolPtr->put(&typeSig(value->type)));
                break;
            }
            case Pool::INT: {
                append1(poolbuf, ClassFile::CONSTANT_Integer);
                int value = w->getValue<int>();
                append4(poolbuf, value);
                break;
            }
            case Pool::STRING: {
                append1(poolbuf, ClassFile::CONSTANT_String);
                Name& name = names->fromString(w->getValue<std::string>());
                append2(poolbuf, poolPtr->put(&name));
                break;
            }
        }
        i++;
    }
    put2(poolbuf, poolCountIdx, (int) pool.size());
}

void ClassWriter::wirteFileAndClose(std::vector<char>& buf) {
    char* cArr = new char[buf.size()];
    for (int i = 0; i < buf.size(); ++i) {
        cArr[i] = buf[i];
    }
    ofs.write(cArr, buf.size());
    delete[] cArr;
    ofs.close();
}

void ClassWriter::appendStr(vector<char>& buf, int len, const char* chars) {
    for (int i = 0; i < len; ++i) {
        buf.push_back(chars[i]);
    }
}

NameAndType::Ptr ClassWriter::nameAndType(SymbolPtr sym) {
    return NameAndType::Ptr(new NameAndType(&sym->name, sym->type));
}

string ClassWriter::dotToSlash(const string& from) {
    std::string flat = from;
    for (int i = 0; i < flat.size(); ++i) {
        if (flat.at(i) == '.') {
            flat.at(i) = '/';
        }
    }
    return flat;
}