//
// Created by withparadox2 on 2016/12/19.
//

#ifndef SIMPLECOMPILER_SCOPE_H
#define SIMPLECOMPILER_SCOPE_H

#include <map>
#include <set>
#include "symbol.h"

class Scope : public std::enable_shared_from_this<Scope> {
private:
    typedef std::map<const Name*, Symbol::Ptr> Map;
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

    Symbol::WeakPtr owner;

    void enter(Symbol::Ptr symbol);

    void remove(const Name& name);

    Scope(Symbol::Ptr owner);

    Symbol::Ptr& lookUp(const Name& name);

    Scope(const Scope& s, const MapPtr& nameToSym);

    /**
     * Share everything except nameToSym which need to be deep copied.
     */
    Scope* dupUnshared();

    /**
     * Share everything, including nameToSym.
     */
    Scope* dup();

    Scope::Ptr leave();
};

class StarImportScope : public Scope {
private:
    StarImportScope();
public:
    StarImportScope& instance();

    void importAll();
};


#endif //SIMPLECOMPILER_SCOPE_H
