//
// Created by withparadox2 on 2017/1/24.
//

#include <memory>
#include "gen.h"
#include "../code/type.h"
#include "../code/Flags.h"
#include "../code/Symtab.h"

#define KEY_GEN "gen"

Gen& Gen::instance() {
    Gen* inst = Context::instance().get<Gen>(KEY_GEN);
    if (inst == nullptr) {
        inst = new Gen();
    }
    return *inst;
}

Gen::Gen() : pool(new Pool),
             syms(Symtab::instance()),
             names(Names::instance()) {
    Context::instance().put(KEY_GEN, this);
}

void Gen::genClass(Env<AttrContext>* env, JCClassDecl* cdef) {
    Env<GenContext>::Ptr localEnv(
            new Env<GenContext>(
                    cdef->shared_from_this(),
                    GenContext::Ptr(new GenContext)
            ));
    for (auto iter = cdef->defs.begin();
         iter != cdef->defs.end(); iter++) {
        genDef(iter->get(), localEnv.get());
    }
}

void Gen::genDef(Tree* tree, Env<GenContext>* env) {
    Env<GenContext>* preEnv = this->env;
    this->env = env;
    tree->accept(this);
    this->env = preEnv;
}

void Gen::visitMethodDef(JCMethodDecl* that) {
    Env<GenContext>::Ptr localEnv(env->dup(that));
    localEnv->enclMethod =
            std::dynamic_pointer_cast<JCMethodDecl>(that->shared_from_this());
    this->pt = that->sym->type->getReturnType();

    //TODO check dimensions

    genMethod(that, localEnv.get(), false);
}

void Gen::genMethod(JCMethodDecl* tree, Env<GenContext>* env, bool fatcode) {
    //Ignore checking params count
    if (tree->body) {
        initCode(tree, env);
        //Ignore checking CodeSizeOverflow
        genStat(tree->body.get(), env);
    }
}

//Assume fatcode = false
void Gen::initCode(JCMethodDecl* tree, Env<GenContext>* env) {
    MethodSymbolPtr meth = tree->sym;
    //TODO construct code
    meth->code = Code::Ptr(new Code);
    this->code = meth->code;

    this->items = Items::Ptr(new Items(code, pool));

    //For non static method, insert `this` as the
    //first parameter.
    if ((tree->mods->flags & Flags::STATIC) == 0) {
        TypePtr selfType = meth->owner->type;

        if (meth->isConstructor() && selfType != syms.objectType) {
            //TODO UninitializedType
        }

        this->code->setDefined(
                this->code->newLocal(
                        VarSymbolPtr(new VarSymbol(Flags::FINAL,
                                                   *names._this,
                                                   selfType,
                                                   meth->owner))));
    }

    for (auto iter = tree->params.begin();
         iter != tree->params.end();
         iter++) {
        this->code->setDefined(
                this->code->newLocal(iter->get()->sym));
    }
    //TODO entryPoint

}

void Gen::genStat(Tree* tree, Env<GenContext>* env) {
    genDef(tree, env);
}

void Gen::visitVarDef(JCVariableDecl* that) {
    VarSymbolPtr v = that->sym;
    code->newLocal(v);
    if (that->init) {
        //TODO check init expr tobe const value
        //lhs = rhs
        //1, Load rhs value into stack
        //2, Store top of stack into this item(lhs)
        genExpr(that->init.get(), v->type)->load();
        items->makeLocalItem(v)->store();
    }
}

void Gen::visitBlock(JCBlock* that) {
    //We will remove registers beyond limit at the end of the block.
    int limit = code->nextreg;
    Env<GenContext>::Ptr localEnv = Env<GenContext>::Ptr(
            env->dup(that,
                     GenContext::Ptr(new GenContext)));
    genStats(that->stats, localEnv.get());

    // TODO End the scope of all block-local variables in variable info.
    if (env->tree->treeTag != Tree::METHODDEF) {
    }
}

void Gen::visitForLoop(JCForLoop* that) {
    int limit = code->nextreg;
    genStats(that->init, env);
    //TODO genLoop
    //TODO endScopes
}

void Gen::visitIf(JCIf* that) {
    int limit = code->nextreg;

}

void Gen::visitExec(JCExpressionStatement* that) {
    genExpr(that->exp.get(), that->exp->type)->drop();
}

void Gen::visitBreak(JCBreak* that) {
}

void Gen::visitContinue(JCContinue* that) {
}

void Gen::visitReturn(JCReturn* that) {
}

void Gen::visitApply(JCMethodInvocation* that) {
}

void Gen::visitNewClass(JCNewClass* that) {
}

void Gen::visitParens(JCParens* that) {
    result = genExpr(that->expr.get(), that->expr->type);
}

void Gen::visitAssign(JCAssign* that) {
}

void Gen::visitConditional(JCConditional* that) {
}

void Gen::visitBinary(JCBinary* that) {
    OperatorSymbolPtr sym =
            std::dynamic_pointer_cast<OperatorSymbol>(that->sym);
    if (sym->opcode == bytecode::string_add) {
        //TODO finish
    } else if (that->treeTag == Tree::AND) {

    } else if (that->treeTag == Tree::OR) {

    } else {
        Item::Ptr od = genExpr(that->lhs.get(),
                               sym->type->getParameterTypes().at(0));
        od->load();
        result = completeBinop(that->lhs, that->rhs, sym);
    }
}

void Gen::visitIndexed(JCArrayAccess* that) {
    genExpr(that->indexed.get(), that->indexed->type)->load();
    genExpr(that->index.get(), syms.intType)->load();
    result = items->makeIndexedItem(that->type);
}

void Gen::visitSelect(JCFieldAccess* that) {
}

void Gen::visitIdent(JCIdent* that) {
    SymbolPtr sym = that->sym;
    if (that->name == *names._this
        || that->name == *names._super) {
        Item::Ptr res = that->name == *names._this
                        ? items->thisItem
                        : items->superItem;
        if (sym->kind == Kind::MTH) {
            res->load();
            res = items->makeMemberItem(sym, true);
        }
        //TODO What will happen if sym is not a method?
        result = res;
    } else if (sym->kind == Kind::VAR && sym->owner->kind == Kind::MTH) {
        result = items->makeLocalItem(
                std::dynamic_pointer_cast<VarSymbol>(sym));
    } else if ((sym->flags & Flags::STATIC) != 0) {
        result = items->makeStaticItem(sym);
    } else {
        items->thisItem->load();
        //Not support inner class, ignore binaryQualifier
        result = items->makeMemberItem(
                sym, (sym->flags & Flags::PRIVATE) != 0);
    }
}

void Gen::visitLiteral(JCLiteral* that) {
    //literal of null
    if (that->type->tag == TypeTags::BOT) {
        code->emitop0(bytecode::aconst_null);

        if (Types::dimensions(pt) > 1) {
            //TODO int[] a = null;
        } else {
            result = items->makeStaticItem(that->type);
        }

    } else {
        result = items->makeImmediateItem(that->type, that->value);
    }
}

void Gen::visitTypeIdent(JCPrimitiveTypeTree* that) {
    //do nothing
}

void Gen::visitTypeArray(JCArrayTypeTree* that) {
    //do nothing
}

void Gen::visitUnary(JCUnary* that) {
}

void Gen::visitNewArray(JCNewArray* that) {
}

Item::Ptr Gen::genExpr(Tree* tree, TypePtr ptr) {
    TypePtr prePt = this->pt;
    this->pt = ptr;
    tree->accept(this);
    this->pt = prePt;
    return result;
}

template<typename T>
void Gen::genStats(std::vector<T>& list, Env<GenContext>* env) {
    for (auto iter = list.begin(); iter < list.end(); iter++) {
        genStat(iter->get(), env);
    }
}

/** Complete generating code for operation, with left operand
 *  already on stack.
 */
Item::Ptr Gen::completeBinop(Tree::Ptr lhs,
                             Tree::Ptr rhs,
                             OperatorSymbolPtr sym) {
    MethodTypePtr optype =
            std::dynamic_pointer_cast<MethodType>(sym->type);


}
