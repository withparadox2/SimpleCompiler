//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include <cassert>

#include "./Parser.h"
#include "../util/error.h"
#include "../code/Flags.h"

using namespace std;

Parser::Parser(Lexer& lexer) : L(lexer), mode(0), lastMode(0), names(Names::instance()) {
    L.nextToken();
}

JCClassDecl* Parser::buildClass() {
    match(Token::CLASS);
    Name& name = ident();
    match(Token::LBRACE);

    Tree::List defs;
    while (L.token() != Token::RBRACE && L.token() != Token::_EOF) {
        defs.push_back(Tree::Ptr(classBodyDecl(name)));
    }

    match(Token::RBRACE);
    return new JCClassDecl(nullptr, name, defs);
}

Tree* Parser::classBodyDecl(Name& className) {
    JCModifiers* modifiers = modifiersOpt();
    JCExpression* type;
    Name* name = &L.name();
    bool isVoid = L.token() == Token::VOID;
    if (isVoid) {
        type = new JCPrimitiveTypeTree(TypeTags::VOID);
        L.nextToken();
    } else {
        type = parseType();
    }

    if (L.token() == Token::LPAREN && type->treeTag == Tree::TYPEIDENT) {
        if (*name != className) {
            error("invalid method declaration; return type required");
        } else {
            //constructor
            return methodDeclaratorRest(modifiers, nullptr, *names.init, true);
        }
    } else {
        name = &ident();
        if (L.token() == Token::LPAREN) {
            return methodDeclaratorRest(modifiers, type, *name, isVoid);
        } else {
            error("method declaration expect (");
            return nullptr;
        }
    }
}

Tree* Parser::methodDeclaratorRest(
        JCModifiers* mods,
        JCExpression* type,
        Name& name,
        bool isVoid) {
    JCVariableDecl::List params = formalParameters();
    if (L.token() != Token::LBRACE) {
        error("need { for method block");
    }
    JCBlock* block1 = block();

    return new JCMethodDecl(mods, type, name, params, block1);
}

JCModifiers* Parser::modifiersOpt() {
    long flags = 0;
    while (true) {
        long flag = 0;
        switch (L.token().id) {
            case Token::ID_PUBLIC:
                flag = Flags::PUBLIC;
                break;
            case Token::ID_STATIC:
                flag = Flags::STATIC;
                break;
            case Token::ID_PRIVATE:
                flag = Flags::PRIVATE;
                break;
        }
        if (flag == 0) {
            break;
        }

        if (flag & flags == flag) {
            error("repeated modifier");
        }

        flags |= flag;
        L.nextToken();
    }

    return new JCModifiers(flags);
}

bool Parser::match(Token& token) {
    match(token, "expected " + token.fullDesc());
}


bool Parser::match(Token& token, string errStr) {
    if (L.token() == token) {
        L.nextToken();
        return true;
    }

    error(errStr);
}

Name& Parser::ident() {
    if (L.token() == Token::IDENTIFIER) {
        Name& name = L.name();
        L.nextToken();
        return name;
    } else {
        match(Token::IDENTIFIER);
    }
}

JCExpression* Parser::parseType() {
    return term(TYPE);
}

JCExpression* Parser::term(int newMode) {
    int prevmode = mode;
    mode = newMode;
    JCExpression* t = term();
    lastMode = mode;
    mode = prevmode;
    return t;
}

JCExpression* Parser::term() {
    JCExpression* t = term1();
    //only support =, not -=, +=, ^=...
    if ((mode & EXPR) != 0 && L.token() == Token::EQ) {
        return termRest(t);
    } else {
        return t;
    }
}

JCExpression* Parser::termRest(JCExpression* t) {
    match(Token::EQ);
    mode = EXPR;//can not be type, since it's an assignment

    //use term() to support syntax like: "int a = b = 2";
    JCExpression* t1 = term();
    return new JCAssign(t, t1);
}

JCExpression* Parser::term1() {
    JCExpression* t = term2();

    if ((mode & EXPR) != 0 && L.token() == Token::QUES) {
        mode = EXPR;
        return term1Rest(t);
    } else {
        return t;
    }
}

JCExpression* Parser::term1Rest(JCExpression* t) {
    if (L.token() == Token::QUES) {
        L.nextToken();
        //TODO why does t1 use term(), while t2 use term1()
        JCExpression* t1 = term();
        match(Token::COLON);
        JCExpression* t2 = term1();
        return new JCConditional(t, t1, t2);
    } else {
        return t;
    }
}

JCExpression* Parser::term2() {
    JCExpression* t = term3();
    if ((mode & EXPR) != 0 && prec(L.token()) >= treeinfo::orPrec) {
        mode = EXPR;
        return term2Rest(t, treeinfo::orPrec);
    } else {
        return t;
    }
}

int Parser::prec(Token& token) {
    int oc = treeinfo::opTag(token);
    return (oc >= 0) ? treeinfo::opPrec(oc) : -1;
}

//TODO using array for efficiency
JCExpression* Parser::term2Rest(JCExpression* t, int minprec) {
    vector<Token*> opStack;
    vector<JCExpression*> odStack;
    odStack.push_back(t);

    Token* topOp = &Token::ERROR;
    while (prec(L.token()) >= minprec) {
        opStack.push_back(topOp);

        topOp = &L.token();
        L.nextToken();

        odStack.push_back(term3());
        // 1 * 2 + 3 => prec(*) > prec(+) =>
        // JCBinary(+, 3, JCBinary(*, 2, 1))
        // remember lhs is the last one being pushed back to stack
        while (opStack.size() > 0 && prec(*topOp) >= prec(L.token())) {
            JCExpression* rhs = odStack.back();//at top
            odStack.pop_back();
            JCExpression* lhs = odStack.back();//at top-1
            odStack.pop_back();

            int opcode = treeinfo::opTag(*topOp);
            odStack.push_back(new JCBinary(opcode, lhs, rhs));//at top-1

            topOp = opStack.back();
            opStack.pop_back();
        }
    }

    if (opStack.size() > 0) {
        error("opstack still has unused opcode.");
    }

    return odStack.at(0);
}

//Primary {Selector} {PostfixOp}, PostifxOp is not supported
JCExpression* Parser::term3() {
    JCExpression* t = nullptr;

    //below is Primary
    switch (L.token().id) {
        case Token::ID_INT:
        case Token::ID_BOOLEAN:
            return bracketOpt(basicType());
        case Token::ID_IDENTIFIER:
            //Identifier { . Identifier } [IdentifierSuffix]
            t = new JCIdent(ident());
            while (true) {
                bool fail = false;
                switch (L.token().id) {
                    case Token::ID_LBRACKET: {
                        L.nextToken();
                        if (L.token() == Token::RBRACKET) {
                            L.nextToken();
                            t = bracketOpt(t);
                            t = new JCArrayTypeTree(t);
                        } else {
                            if ((mode & EXPR) != 0) {
                                JCExpression* t1 = term();
                                t = new JCArrayAccess(t, t1);
                            }
                            match(Token::RBRACKET);
                        }
                        fail = true;
                        break;
                    }
                    case Token::ID_DOT: {
                        L.nextToken();

                        //It's impossible to be indent.this(arg1, arg2),
                        //i.e. method invocation here is illegal
                        if (L.token() == Token::THIS) {
                            mode = EXPR;
                            t = new JCFieldAccess(t, *names._this);
                            fail = true;
                            L.nextToken();
                            break;
                        }

                        //not set `fail` to false, continue to parse `ident {. ident}`
                        t = new JCFieldAccess(t, ident());
                        break;
                    }
                    case Token::ID_LPAREN: {
                        t = arguments(t);
                        fail = true;
                        break;
                    }
                    default:
                        fail = true;
                        break;
                }
                if (fail) break;
            }
            return t;//TODO without selector?
        case Token::ID_INTLITERAL:
        case Token::ID_TRUE:
        case Token::ID_FALSE:
        case Token::ID_NULL_:
        case Token::ID_STRINGLITERAL:
            if ((mode & EXPR) != 0) {
                mode = EXPR;
                t = literal("");
            } else {
                error("literal must be an expression, @ " + L.bufStr);
            }
            break;
        case Token::ID_LPAREN: {
            if ((mode & EXPR) != 0) {
                L.nextToken();
                //TODO why don't just use term() for simplicity?
                t = termRest(term1Rest(term2Rest(term3(), treeinfo::opPrec(treeinfo::orPrec))));
                match(Token::RPAREN);
                t = new JCParens(t);
            } else {
                error("( must be an expression");
            }
            break;
        }
        case Token::ID_NEW:
            if ((mode & EXPR) != 0) {
                mode = EXPR;
                L.nextToken();
                t = creator();
            } else {
                error("new Object must be an expression");
            }
            break;
        case Token::ID_PLUS:
        case Token::ID_SUB:
            if ((mode & EXPR) != 0) {
                mode = EXPR;
                L.nextToken();
                if (L.token() == Token::INTLITERAL) {
                    t = literal(names.hyphen->desc);
                } else {
                    return new JCUnary(unoptag(L.token()), t);
                }
            } else {
                error("no type for + or -");
            }
            break;
        case Token::ID_THIS:
            //this.func(); implemented in 'Selector', see while loop outside this switch
            //this.name;   same as above
            //this();      called in constructor, not supported!!!
            //this         yes, just this
            t = new JCIdent(*names._this);
            break;
        default:
            //expressions not supported:
            //Super
            error("not supported syntax " + L.token().fullDesc());
    }

    /**  Selector      = "." Ident [Arguments]
     *                 | "." THIS
     *                 | "." SUPER SuperSuffix【not supported】
     *                 | "." NEW [TypeArguments] InnerCreator【not supported】
     *                 | "[" Expression "]"
     */

    //below is Selector
    while (true) {
        if (L.token() == Token::DOT) {
            L.nextToken();
            switch (L.token().id) {
                case Token::ID_IDENTIFIER:
                    t = new JCFieldAccess(t, ident());
                    t = argumentsOpt(t);
                    break;
                default:
                    error("not super or new in selector");
                    break;

            }
        } else if (L.token() == Token::LBRACKET) {
            L.nextToken();
            //mode may be (EXPR|TYPE), first check type, then check expr
            if ((mode & TYPE) != 0) {
                if (L.token() == Token::RBRACKET) {
                    mode = TYPE;
                    t = bracketOpt(t);

                    //no way to be other things, just return.
                    return new JCArrayTypeTree(t);
                }
            }

            if ((mode & EXPR) != 0) {
                mode = EXPR;
                JCExpression* t1 = term(EXPR);
                t = new JCArrayAccess(t, t1);
            }

            //may be arr[e1][e2] or arr[e1].fun(), so continue to check selector
        } else {
            break;
        }
    }
    return t;
}

//For now, only support array of primitive type, i.e. int and boolean
JCExpression* Parser::creator() {
    switch (L.token().id) {
        case Token::ID_INT:
        case Token::ID_BOOLEAN:
            return arrayCreatorRest(basicType());
        default:
            JCExpression* t = qualident();
            if (L.token() == Token::LPAREN) {
                JCExpression::List args = arguments();
                return new JCNewClass(t, args);
            } else {
                error("missing ( for creating class");
            }
    }
}

/** ArrayCreatorRest = "[" ( "]" BracketsOpt ArrayInitializer
 *                         | Expression "]" {"[" Expression "]"} BracketsOpt )
 *
 *  For now, only support int[][] arr = new int[3][4] and int[][] arr = new int[3][]
 *  also array init: int[] arr = new int[]{1, 2, 3};
 */
JCExpression* Parser::arrayCreatorRest(JCExpression* elemtype) {
    match(Token::LBRACKET);
    if (L.token() == Token::RBRACKET) {
        L.nextToken();
        elemtype = bracketOpt(elemtype);
        if (L.token() == Token::LBRACE) {
            return arrayInitializer(elemtype);
        } else {
            error("missing dimension for array");
        }
    } else {
        JCExpression::List dimens;
        dimens.push_back(JCExpression::Ptr(term(EXPR)));
        match(Token::RBRACKET);
        while (L.token() == Token::LBRACKET) {
            L.nextToken();
            if (L.token() == Token::RBRACKET) {
                //int[][][] arr = new int[3][][];
                elemtype = bracketsOptCont(elemtype);
                assert(L.token() != Token::LBRACKET);
            } else {
                dimens.push_back(JCExpression::Ptr(term(EXPR)));
                match(Token::RBRACKET);
            }
        }

        JCExpression::List elems;
        return new JCNewArray(elemtype, dimens, elems);
    }
}


JCExpression* Parser::literal(const string& prefiex) {
    JCExpression* t = nullptr;
    switch (L.token().id) {
        case Token::ID_INTLITERAL:
            try {
                int value = std::stoi(prefiex + L.bufStr);
                t = new JCLiteral(TypeTags::INT, value);
            } catch (std::exception& e) {
                error(L.bufStr + " can not be converted to int.");
            }
            break;
        case Token::ID_STRINGLITERAL:
            t = new JCLiteral(TypeTags::CLASS, (string) L.bufStr);
            break;
        case Token::ID_NULL_:
            t = new JCLiteral(TypeTags::BOT, 0);
            break;
        case Token::ID_TRUE:
        case Token::ID_FALSE:
            t = new JCLiteral(TypeTags::BOOLEAN,
                              L.token() == Token::TRUE ? 1 : 0);
        default:
            error("wrong literal " + L.token().fullDesc());
    }
    L.nextToken();
    return t;
}

JCExpression* Parser::basicType() {
    JCPrimitiveTypeTree* t = new JCPrimitiveTypeTree(typeTag(L.token()));
    L.nextToken();
    return t;
}

int Parser::typeTag(Token& token) {
    switch (token.id) {
        case Token::ID_INT:
            return TypeTags::INT;
        case Token::ID_BOOLEAN:
            return TypeTags::BOOLEAN;
        default:
            return -1;
    }
}

JCExpression* Parser::bracketOpt(JCExpression* e) {
    if (L.token() == Token::LBRACKET) {
        L.nextToken();
        e = bracketsOptCont(e);
    }
    return e;
}

JCExpression* Parser::bracketsOptCont(JCExpression* e) {
    match(Token::RBRACKET);
    e = bracketOpt(e);
    return new JCArrayTypeTree(e);
}

JCVariableDecl::List Parser::formalParameters() {
    JCVariableDecl::List vec;
    match(Token::LPAREN);
    if (L.token() != Token::RPAREN) {
        vec.push_back(JCVariableDecl::Ptr(formalParameter()));
        while (L.token() == Token::COMMA) {
            L.nextToken();
            vec.push_back(JCVariableDecl::Ptr(formalParameter()));
        }
    }
    match(Token::RPAREN);
    return vec;
}

JCVariableDecl* Parser::formalParameter() {
    JCExpression* type = parseType();
    return new JCVariableDecl(ident(), type);
}

JCExpression* Parser::qualident() {
    JCExpression* t = new JCIdent(ident());
    while (L.token() == Token::DOT) {
        L.nextToken();
        t = new JCFieldAccess(t, ident());
    }
    return t;
}

JCExpression::List Parser::arguments() {
    match(Token::LPAREN);
    JCExpression::List args;
    if (L.token() != Token::RPAREN) {
        args.push_back(JCExpression::Ptr(term(EXPR)));
        while (L.token() == Token::COMMA) {
            L.nextToken();
            args.push_back(JCExpression::Ptr(term(EXPR)));
        }
    }
    match(Token::RPAREN);
    return args;
}

JCMethodInvocation* Parser::arguments(JCExpression* t) {
    JCExpression::List args = arguments();
    return new JCMethodInvocation(args, t);
}

JCExpression* Parser::argumentsOpt(JCExpression* t) {
    if (L.token() == Token::LPAREN) {
        return arguments(t);
    } else {
        return t;
    }
}


JCExpression* Parser::arrayInitializer(JCExpression* t) {
    match(Token::LBRACE);
    JCExpression::List elems;
    if (L.token() != Token::RBRACE) {
        elems.push_back(JCExpression::Ptr(term(EXPR)));
        while (L.token() == Token::COMMA) {
            L.nextToken();
            elems.push_back(JCExpression::Ptr(term(EXPR)));
        }
    }
    match(Token::RBRACE);
    JCExpression::List dimens;
    return new JCNewArray(t, dimens, elems);
}

JCBlock* Parser::block() {
    match(Token::LBRACE);
    JCStatement::List vec = blockStatements();
    match(Token::RBRACE);
    return new JCBlock(vec);
}

/**
 * LocalVariableDeclarationStatement
 * Statement
 */
JCStatement::List Parser::blockStatements() {
    JCStatement::List stats;
    while (true) {
        switch (L.token().id) {
            case Token::ID_RBRACE:
            case Token::ID_EOF:
                return stats;
            case Token::ID_IF:
            case Token::ID_FOR:
            case Token::ID_RETURN:
                stats.push_back(JCStatement::Ptr(parseStatement()));
                break;
            default:
                JCExpression* t = term(TYPE | EXPR);
                //local var decl
                if ((lastMode & TYPE) != 0 && L.token() == Token::IDENTIFIER) {
                    Name& name = ident();
                    JCExpression* init = nullptr;
                    if (L.token() == Token::EQ) {
                        L.nextToken();
                        init = term(EXPR);
                    }
                    stats.push_back(JCStatement::Ptr(new JCVariableDecl(name, t, init)));
                    match(Token::SEMI);
                } else {
                    stats.push_back(JCStatement::Ptr(new JCExpressionStatement(t)));
                    match(Token::SEMI);
                }
                break;
        }
    }
    return stats;
}


JCStatement* Parser::parseStatement() {
    switch (L.token().id) {
        case Token::ID_LBRACE:
            return block();
        case Token::ID_IF: {
            L.nextToken();
            JCExpression* cond = parExpression();
            JCStatement* thenpart = parseStatement();
            JCStatement* elsepart = nullptr;
            if (L.token() == Token::ELSE) {
                L.nextToken();
                elsepart = parseStatement();
            }
            return new JCIf(cond, thenpart, elsepart);
        }
        case Token::ID_FOR: {
            //todo support for each
            L.nextToken();
            match(Token::LPAREN);
            JCStatement::List inits = forInit();
            match(Token::SEMI);
            JCExpression* cond = L.token() == Token::SEMI ? nullptr : term(EXPR);
            match(Token::SEMI);
            JCExpressionStatement::List steps = forUpdate();
            match(Token::RPAREN);

            JCStatement* body = parseStatement();

            return new JCForLoop(inits, cond, steps, body);
        }
        case Token::ID_BREAK: {
            L.nextToken();
            match(Token::SEMI);
            return new JCBreak;
        }
        case Token::ID_CONTINUE: {
            L.nextToken();
            match(Token::SEMI);
            return new JCContinue;
        }

        case Token::ID_RETURN: {
            L.nextToken();
            JCExpression* e = term(EXPR);
            match(Token::SEMI);
            return new JCReturn(e);
        }

        default:
            error("not support " + L.token().fullDesc() + " in statement");
    }
    return nullptr;
}

JCExpression* Parser::parExpression() {
    match(Token::LPAREN);
    JCExpression* t = term(EXPR);
    match(Token::RPAREN);
    return t;
}

JCStatement::List Parser::forInit() {
    JCStatement::List stats;
    if (L.token() == Token::SEMI) {
        return stats;
    }

    //only support syntax like: for(int a = 0; ...) or for(a = 2; ...)
    //not for(int a=0, b=2)
    JCExpression* t = term(EXPR | TYPE);

    if ((lastMode & TYPE) != 0 && L.token() == Token::IDENTIFIER) {
        Name& name = ident();
        JCExpression* init = nullptr;
        if (L.token() == Token::EQ) {
            L.nextToken();
            init = term(EXPR);
        }
        stats.push_back(JCStatement::Ptr(new JCVariableDecl(name, t, init)));
    } else {
        stats.push_back(JCStatement::Ptr(new JCExpressionStatement(t)));
    }
    return stats;
}


JCExpressionStatement::List Parser::forUpdate() {
    JCExpressionStatement::List stats;
    if (L.token() == Token::RPAREN) {
        return stats;
    }

    stats.push_back(JCExpressionStatement::Ptr(new JCExpressionStatement(term(EXPR))));
    return stats;
}

int Parser::unoptag(Token& token) {
    switch (token.id) {
        case Token::ID_SUB:
            return Tree::NEG;
        case Token::ID_PLUS:
            return Tree::POS;
        default:
            error("not support unary : " + token.fullDesc());
    }
}



