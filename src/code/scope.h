//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SCOPE_H
#define SIMPLECOMPILER_SCOPE_H

#include <map>
#include <set>
#include "symbols.h"
#include "types.h"

class Name;

class Scope : public std::enable_shared_from_this<Scope> {
private:
    static SymbolList emptyList;
    typedef std::map<const Name*, SymbolList> Map;
    typedef std::shared_ptr<Map> MapPtr;
    /**
     * May be shared with other scope, using elems to track
     * real items inserted into current scope.
     */
    MapPtr nameToSym;
    std::set<const Name*> elems;
public:
    typedef std::shared_ptr<Scope> Ptr;

    std::shared_ptr<Scope> next;

    SymbolWeakPtr owner;

    void enter(SymbolPtr symbol);

    void remove(const Name& name);

    Scope(SymbolPtr owner);

    SymbolPtr& lookUp(const Name& name);

    SymbolList& lookUpList(const Name& name);

    Scope(const Scope& s, const MapPtr& nameToSym);

    /**
     * Share everything except nameToSym which need to be deep copied.
     */
    Scope::Ptr dupUnshared();

    /**
     * Share everything, including nameToSym.
     * Remember to call leave().
     */
    Scope::Ptr dup();

    Scope::Ptr leave();

    void printTable();
};

class StarImportScope : public Scope {
private:
    StarImportScope();
public:
    static StarImportScope& instance();

    void importAll();
};


#endif //SIMPLECOMPILER_SCOPE_H
