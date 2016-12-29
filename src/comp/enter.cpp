//
// Created by withparadox2 on 2016/12/19.
//

#include "enter.h"
#include "../util/error.h"

Enter &Enter::instance() {
    static Enter inst;
    return inst;
}

void Enter::complete(Tree *tree) {
    tree->accept(*this);
}

void Enter::visitClassDef(JCClassDecl &that) {

}

void Enter::visitMethodDef(JCMethodDecl &that) {

}

void Enter::visitTree(Tree &that) {
    //do nothing
}

Enter::Enter() : reader(ClassReader::instance()) {
}

