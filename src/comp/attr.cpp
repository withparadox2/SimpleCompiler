//
// Created by withparadox2 on 2017/1/8.
//

#include <iostream>
#include "attr.h"
#include "../util/log.h"
#include "../code/Symtab.h"
#include "enter.h"
#include "../code/type.h"

Attr& Attr::instance() {
    static Attr attr;
    return attr;
}

Type::Ptr Attr::attribType(Tree* tree, Env* env) {
    return attribTree(tree, env, Kind::TYP);
}


TypePtr Attr::attribStat(Tree* tree, Env* env) {
    return attribTree(tree, env, Kind::NIL);
}


Type::Ptr Attr::attribTree(Tree* tree, Env* env, int pkind) {
    int preKind = this->pKind;
    Env* preEnv = this->env;
    this->pKind = pkind;
    this->env = env;

    tree->accept(this);

    this->pKind = preKind;
    this->env = preEnv;
    return result;
}

Attr::Attr() : pKind(Kind::ERR), env(nullptr), syms(Symtab::instance()) {

}

void Attr::visitVarDef(JCVariableDecl* that) {
    if (!that->sym) {
        that->sym = VarSymbol::Ptr(new VarSymbol(0, that->name, that->type, syms.noSymbol));
    }
}

void Attr::visitIdent(JCIdent* that) {
    Symbol::Ptr sym;
    //TODO why it is necessary to check kind()
    if (that->sym && that->sym->kind != Kind::VAR) {
        sym = that->sym;
    } else {
        sym = Symbol::Ptr(resolveIdent(env, that->name, pKind));
    }
}

void Attr::visitTypeArray(JCArrayTypeTree* that) {
    TypePtr eType = attribType(that->elementType.get(), this->env);
    result = TypePtr(new ArrayType(eType, syms.arrayClass));
    that->type = result;
}

Symbol::Ptr Attr::resolveIdent(Env* env, const Name& name, int kind) {
    if ((kind & Kind::TYP) != 0) {
        return findType(env, name);
    }
    return syms.noSymbol;
}

Symbol::Ptr Attr::findType(Env* env, const Name& name) {
    for (Env* env1 = env; env1->outer; env1 = env1->outer.get()) {
        Symbol::Ptr sym = env1->info->scope->lookUp(name);
        if (sym && sym->kind == Kind::TYP) {
            return sym;
        }
    }

    StarImportScope& gScope = StarImportScope::instance();

    Symbol::Ptr sym = gScope.lookUp(name);
    if (sym) {
        log("find type : " + sym->name.desc);
    }
    return syms.noSymbol;
}

void Attr::visitTypeIdent(JCPrimitiveTypeTree* that) {
    result = syms.typeOfTag[that->typetag];
    that->type = result;
}

void Attr::attrib(Env* env) {
    attribClass(env->enclClass->sym);
}

void Attr::attribClass(ClassSymbol::Ptr c) {
    Env* env = enter().typeEnvs.at(c);
    JCClassDecl::Ptr tree = env->enclClass;
    for (auto iter = tree->defs.begin(); iter != tree->defs.end(); iter++) {
        (*iter)->accept(this);
    }
}

void Attr::visitMethodDef(JCMethodDecl* that) {
    log("attr method : " + that->name.desc);
    //ignore any check.
    MethodSymbol::Ptr m = that->sym;
    JCMethodDecl::Ptr treePtr = std::dynamic_pointer_cast<JCMethodDecl>
            (that->shared_from_this());
    Env::Ptr localEnv(enter().methodEnv(treePtr, env));

    // Attribute all value parameters.
    for (auto iter = that->params.begin(); iter != that->params.end(); iter++) {
        attribStat(iter->get(), localEnv.get());
    }



}

Enter& Attr::enter() {
    return Enter::instance();
}

void Attr::visitBlock(JCBlock* that) {
}

void Attr::visitForLoop(JCForLoop* that) {
}

void Attr::visitIf(JCIf* that) {
}

void Attr::visitExec(JCExpressionStatement* that) {
}

void Attr::visitBreak(JCBreak* that) {
}

void Attr::visitContinue(JCContinue* that) {
}

void Attr::visitReturn(JCReturn* that) {
}

void Attr::visitApply(JCMethodInvocation* that) {
}

void Attr::visitNewClass(JCNewClass* that) {
}

void Attr::visitParens(JCParens* that) {
}

void Attr::visitAssign(JCAssign* that) {
}

void Attr::visitConditional(JCConditional* that) {
}

void Attr::visitBinary(JCBinary* that) {
}

void Attr::visitIndexed(JCArrayAccess* that) {
}

void Attr::visitSelect(JCFieldAccess* that) {
}

void Attr::visitLiteral(JCLiteral* that) {
}

void Attr::visitUnary(JCUnary* that) {
}

void Attr::visitNewArray(JCNewArray* that) {
}

void Attr::visitMethodInvocation(JCMethodInvocation* that) {
}
