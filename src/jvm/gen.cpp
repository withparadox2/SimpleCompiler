//
// Created by withparadox2 on 2017/1/24.
//

#include <memory>
#include "gen.h"
#include "../code/Symtab.h"
#include "../util/error.h"
#include "../util/tools.h"

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
             names(Names::instance()),
             attr(Attr::instance()) {
    Context::instance().put(KEY_GEN, this);
    vector<TypePtr> args;
    methodType = MethodTypePtr(new MethodType(args, TypePtr(nullptr), syms.methodClass));
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
        this->code->endScopes(limit);
    }
}

void Gen::visitForLoop(JCForLoop* that) {
    int limit = code->nextreg;
    genStats(that->init, env);
    genLoop(that, that->body.get(), that->cond.get(), that->step, true);
    code->endScopes(limit);
}

void Gen::visitIf(JCIf* that) {
    int limit = code->nextreg;
    CondItemPtr c = genCond(treeinfo::skipParens(that->cond.get()));
    Chain* elseChain = c->jumpFalse();
    Chain* thenExit = nullptr;

    genStat(that->thenPart.get(), env);
    thenExit = code->branch(bytecode::goto_);

    code->resolve(elseChain, code->cp);

    genStat(that->elsePart.get(), env);
    code->resolve(thenExit, code->cp);

    code->endScopes(limit);
}

void Gen::visitExec(JCExpressionStatement* that) {
    genExpr(that->exp.get(), that->exp->type)->drop();
}

void Gen::visitBreak(JCBreak* that) {
}

void Gen::visitContinue(JCContinue* that) {
}

void Gen::visitReturn(JCReturn* that) {
    int limit = code->nextreg;
    if (that->expr) {
        Item::Ptr r = genExpr(that->expr.get(), pt)->load();
        r->load();
        code->emitop0(bytecode::ireturn
                      + Code::truncate(Code::typecode(pt.get())));
    } else {
        code->emitop0(bytecode::ireturn);
    }
    code->endScopes(limit);
}

void Gen::visitApply(JCMethodInvocation* that) {
    Item::Ptr method = this->genExpr(that->meth.get(), this->methodType);
    MethodTypePtr mPtr = dynamic_pointer_cast<MethodType>(treeinfo::symbol(that->meth.get())->type);
    this->genArgs(that->args, mPtr->argtypes);
    result = method->invoke();
}

void Gen::visitNewClass(JCNewClass* that) {
    this->code->emitop2(bytecode::new_, this->makeRef(that->type));
    this->code->emitop0(bytecode::dup);
    this->genArgs(that->arguments, that->argtypes);
    this->items->makeMemberItem(that->constructor, true)->invoke();
    result = this->items->makeStackItem(that->type);
}

void Gen::visitParens(JCParens* that) {
    result = genExpr(that->expr.get(), that->expr->type);
}

void Gen::visitAssign(JCAssign* that) {
    Item::Ptr l = this->genExpr(that->lhs.get(), that->lhs->type);
    genExpr(that->rhs.get(), that->lhs->type)->load();
    this->result = this->items->makeAssignItem(l);
    //Calling drop() in visitExec will store value to lhs
}

void Gen::visitConditional(JCConditional* that) {
}

void Gen::visitBinary(JCBinary* that) {
    OperatorSymbolPtr sym =
            std::dynamic_pointer_cast<OperatorSymbol>(that->sym);
    if (sym->opcode == bytecode::string_add) {
        code->emitop2(bytecode::new_, makeRef(syms.stringBuilderType));
        code->emitop0(bytecode::dup);

        callMethod(syms.stringBuilderType, *names.init, TypeList(), false);

        appendStrings(that);

        callMethod(syms.stringBuilderType, *names.toString, TypeList(), false);

        result = items->makeStackItem(syms.stringType);
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
    SymbolPtr sym = that->sym;
    if (that->selector == *this->names._class) {
        //TODO
        return;
    }

    SymbolPtr ssym = treeinfo::symbol(that->selected.get());

    Item::Ptr base = this->genExpr(that->selected.get(), that->selected->type);

    //TODO binaryQualifier
    if ((sym->flags & Flags::STATIC) != 0) {
        base->drop();
        result = items->makeStaticItem(sym);
    } else {
        base->load();
        if (sym == syms.lengthVar) {
            this->code->emitop0(bytecode::arraylength);
            result = items->makeStaticItem(sym);
        } else {
            result = items->makeMemberItem(sym, (sym->flags & Flags::PRIVATE) != 0);
        }
    }
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
            result = items->makeStackItem(that->type);
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
    if (that->elems.size() > 0) {
        // We create a one dimentional array and initialize each item with
        // respected value stored in JCNewArray.elems

        // Load array's implicit length to stack, then call `newarray`
        this->loadIntConst((int) that->elems.size());
        Item::Ptr arr = this->makeNewArray(that->type, 1);

        TypePtr elemType = Types::elemtype(that->type);

        int i = 0;
        // Now, arr is on the top of stack, we have to duplicate arr every time
        for (auto iter = that->elems.begin(); iter != that->elems.end(); iter++) {
            // Load arr again
            arr->duplicate();
            // Load item's index
            this->loadIntConst(i);
            i++;
            // Load item's value
            this->genExpr(iter->get(), elemType);
            this->items->makeIndexedItem(elemType)->load();
        }
        this->result = arr;
    } else {
        // Load values of dimens to stack, then call `newarray`
        for (auto iter = that->dimens.begin(); iter < that->dimens.end(); iter++) {
            // Type of each dimension must be int
            genExpr(iter->get(), syms.intType)->load();
        }
        this->result = makeNewArray(that->type, (int) that->dimens.size());
    }
}


Item::Ptr Gen::makeNewArray(TypePtr t, int ndims) {
    TypePtr elemType = Types::elemtype(t);
    if (Types::dimensions(t) > 0xff) {
        error("Dimension of Array can not exceed 255");
    }

    int elemCode = Code::arraycode(elemType.get());
    if (elemCode == 0 || (elemCode == 1 && ndims == 1)) {
        //TODO new Object[]; or new int[1][]{1, 2, 3};
    } else if (elemCode == 1) {
        //TODO new int[1][2];
    } else {
        //Simple, new int[2]; or new int[]{1, 2, 3};
        this->code->emitNewarray(elemCode, t);
    }
    return items->makeStackItem(t);
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
    int opcode = sym->opcode;

    TypePtr rtype = sym->type->getParameterTypes().at(0);
    genExpr(rhs.get(), rtype)->load();

    return items->makeCondItem(opcode);
}

//TODO this is too slow
void Gen::loadIntConst(int n) {
    this->items->makeImmediateItem(syms.intType,
                                   IValueHolder::Ptr(new ValueHolder<int>(n)))->load();
}

void Gen::genArgs(JCExpression::List& list, TypeList& typeList) {
    auto aIter = list.begin();
    auto pIter = typeList.begin();
    while (aIter != list.end() && pIter != typeList.end()) {
        this->genExpr(aIter->get(), *pIter)->load();
        aIter++;
        pIter++;
    }
}

int Gen::makeRef(TypePtr type) {
    if (type->tag == TypeTags::CLASS) {
        return pool->put(type->tsym.lock());
    } else {
        return pool->put(type);
    }
}

void Gen::genLoop(JCStatement* loop,
                  JCStatement* body,
                  JCExpression* cond,
                  JCExpressionStatement::List step,
                  bool testFirst) {
    Env<GenContext>::Ptr localEnv(env->dup(loop, GenContext::Ptr(new GenContext)));

    int startpc = code->cp;

    if (testFirst) {
        CondItem::Ptr c;
        if (cond != nullptr) {
            c = genCond(treeinfo::skipParens(cond));
        } else {
            //TODO makeCondItem
        }
        Chain* loopDone = c->jumpFalse();
        code->resolve(c->trueJumps);
        genStat(body, localEnv.get());
        genStats(step, localEnv.get());

        code->resolve(code->branch(bytecode::goto_), startpc);
        code->resolve(loopDone, code->cp);
    }
}

CondItem::Ptr Gen::genCond(Tree* tree) {
    Tree* innerTree = treeinfo::skipParens(tree);
    if (innerTree->treeTag == Tree::CONDEXPR) {
        //TODO
    } else {
        CondItem::Ptr result = genExpr(tree, syms.booleanType)->mkCond();
        return result;
    }
}

void Gen::callMethod(TypePtr site, Name& name, TypeList argtypes, bool isStatic) {
    SymbolPtr msym = attr.findMethod(nullptr, site, name, argtypes, true);
    if (isStatic) items->makeStaticItem(msym)->invoke();
    else items->makeMemberItem(msym, name == *names.init)->invoke();
}

void Gen::appendStrings(Tree* tree) {
    tree = treeinfo::skipParens(tree);
    if (tree->treeTag == Tree::PLUS) {
        JCBinary* op = dynamic_cast<JCBinary*>(tree);
        if (op->sym->kind == Kind::MTH
            && dynamic_cast<OperatorSymbol*>(op->sym.get())->opcode == bytecode::string_add) {
            appendStrings(op->lhs.get());
            appendStrings(op->rhs.get());
            return;
        }
    }

    genExpr(tree, tree->type)->load();
    appendString(tree);
 }

void Gen::appendString(Tree* tree) {
    callMethod(syms.stringBuilderType, *names.append, ofList(tree->type), false);
}


