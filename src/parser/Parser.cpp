//
// Created by withparadox2 on 2016/8/23.
//
#include <iostream>
#include "Parser.h"
#include "../util/error.h"
#include "../code/Flags.h"
#include "../code/TypeTags.h"
#include "../util/names.h"

using namespace std;

Parser::Parser(Lexer &lexer) : L(lexer), mode(0), lastMode(0) {}

Tree *Parser::parse() {
    return buildClass();
}

JCClassDecl *Parser::buildClass() {
    match(Token::CLASS);
    Name &name = ident();
    match(Token::LBRACE);

    vector<Tree *> *defs = new vector<Tree *>();
    while (L.token() != Token::RBRACE && L.token() != Token::_EOF) {
        defs->push_back(classBodyDecl(Name & className));
    }

    match(Token::RBRACE);
    return new JCClassDecl(nullptr, name, defs);
}

Tree *Parser::classBodyDecl(Name &className) {
    JCModifiers *modifiers = modifiersOpt();
    JCExpression *type;
    Name *name = &L.name();
    bool isVoid = L.token() == Token::VOID;
    if (isVoid) {
        type = new JCPrimitiveTypeTree(TypeTags::VOID);
        L.nextToken();
    } else {
        type = parseType();
    }

    if (L.token() == Token::LPAREN && type->treeTag == Tree::TYPEIDENT) {
        if (*name != className) {
            errno("invalid method declaration; return type required");
        } else {
            //constructor
            return methodDeclaratorRest(modifiers, nullptr, Names::instance().init, true);
        }
    } else {
        L.nextToken();
        name = ident();
        match(Token::LPAREN);//TODO support var declaration
        return methodDeclaratorRest(modifiers, type, name, isVoid);
    }
}

Tree *Parser::methodDeclaratorRest(
        JCModifiers *mods,
        JCExpression *type,
        Name &name,
        boolean isVoid) {
    vector<JCExpression *> *params = formalParameters();
    if (L.token() != Token::LBRACE) {
        error("need { for method block");
    }
    JCBlock *block = block();

    return new JCMethodDecl(mods, type, &name, params, block);
}

JCModifiers *Parser::modifiersOpt() {
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
            errno("repeated modifier");
        }

        flags |= flag;
        L.nextToken();
    }

    return new JCModifiers(flags);
}

bool Parser::match(Token &token) {
    if (L.token() == token) {
        L.nextToken();
        return true;
    }

    error("expected " + token.fullDesc());
}

Name &Parser::ident() {
    if (L.token() == Token::IDENTIFIER) {
        Name &name = L.name();
        L.nextToken();
        return name;
    } else {
        match(Token::IDENTIFIER);
    }
}

JCExpression *Parser::parseType() {
    return term(TYPE);
}

JCExpression *Parser::term(int newMode) {
    int prevmode = mode;
    mode = newMode;
    JCExpression *t = term();
    lastmode = mode;
    mode = prevmode;
    return t;
}

JCExpression *Parser::term() {
    JCExpression *t = term1();
    //only support =, not -=, +=, ^=...
    if ((mode & EXPR) != 0 && L.token() == Token::EQ) {
        return termRest(t);
    } else {
        return t;
    }
}

JCExpression *Parser::termRest(JCExpression *t) {
    match(Token::EQ);
    mode = EXPR;//can not be type, since it's an assignment

    //use term() to support syntax like: "int a = b = 2";
    JCExpression t1 = term();
    return JCAssign(t, t1);
}

JCExpression *Parser::term1() {
    JCExpression *t = term2();

    if ((mode & EXPR) != 0 && L.token() == Token::QUES) {
        mode = EXPR;
        return term1Rest(t);
    } else {
        return t;
    }
}

JCExpression *Parser::term1Rest(JCExpression *t) {
    if (L.token() == Token::QUES) {
        L.nextToken();
        //TODO why does t1 use term(), while t2 use term1()
        JCExpression *t1 = term();
        match(Token::COLON);
        JCExpression *t2 = term1();
        return new JCConditional(t, t1, t2);
    } else {
        return t;
    }
}

JCExpression *Parser::term2() {
    JCExpression *t = term3();
    if ((mode & EXPR) != 0 && prec(L.token()) >= treeinfo::orPrec) {
        mode == EXPR;
        return term2Rest(t, treeinfo::orPrec);
    } else {
        return t;
    }
}

int Parser::prec(Token &token) {
    int oc = treeinfo::opTag(token);
    return (oc >= 0) ? treeinfo::opPrec(oc) : -1;
}

//TODO using array for efficiency
JCExpression *Parser::term2Rest(JCExpression *t, int minprec) {
    vector<Token *> opStack;
    vector<JCExpression *> odStack;
    odStack.push_back(t);

    Token *topOp = &Token::ERROR;
    while (prec(L.token()) >= minprec) {
        opStack.push_back(topOp);

        topOp = &L.token();
        L.nextToken();

        odStack.push_back(term3());
        // 1 * 2 + 3 => prec(*) > prec(+) =>
        // JCBinary(+, 3, JCBinary(*, 2, 1))
        // remember lhs is the last one being pushed back to stack
        while (opStack.size() > 0 && prec(*topOp) >= prec(L.token())) {
            Token *lhs = odStack.back();//at top
            odStack.pop_back();
            Token *rhs = odStack.back();//at top-1
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

JCExpression *Parser::term3() {
    switch (L.token().id) {
        case Token::ID_INT:
        case Token::ID_BOOLEAN:
            return bracketOpt(basicType());
        case Token::ID_IDENTIFIER:
            JCExpression *t = new JCIdent(ident());
            while (true) {
                bool fail = false;
                switch (L.token().id) {
                    case Token::ID_LBRACKET: {
                        L.nextToken();
                        if (L.token() == Token::RBRACKET) {
                            t = bracketOpt(t);
                        } else {
                            if ((mode & EXPR) != 0) {
                                JCExpression *t1 = term();
                                t = new JCArrayAccess(t, t1);
                            }
                            match(Token::RBRACKET);
                        }
                        fail = true;
                        break;
                    }
                    case Token::ID_DOT: {
                        L.nextToken();
                        t = new JCFieldAccess(t, ident());
                        break;
                    }
                    default:
                        fail = true;
                        break;
                }
                if (fail) break;
            }
            return t;
        case Token::ID_INTLITERAL:
        case Token::ID_TRUE:
        case Token::ID_FALSE:
        case Token::ID_NULL_:
        case Token::ID_STRINGLITERAL:
            if ((mode & EXPR) != 0) {
                mode = EXPR;
                t = literal();
            } else {
                error("literal must be an expression");
            }
            break;

    }
    //TODO handle this
}

JCExpression *Parser::literal() {
    JCExpression *t = nullptr;
    switch (L.token().id) {
        case Token::ID_INTLITERAL:
            //TODO check exception?
            int value = atoi(L.bufStr.c_str());
            t = new JCLiteral<int>(TypeTags::INT, value);
            break;
        case Token::ID_STRINGLITERAL:
            t = new JCLiteral<string>(TypeTags::CLASS, L.bufStr);
            break;
        case Token::ID_NULL_:
            t = new JCLiteral<int>(TypeTags::BOT, 0);
            break;
        case Token::ID_TRUE:
        case Token::ID_FALSE:
            t = new JCLiteral<int>(TypeTags::BOOLEAN,
                                   L.token() == Token::TRUE ? 1 : 0);
        default:
            error("wrong literal " + L.token().fullDesc());
    }
    L.nextToken();
    return t;
}

JCExpression *Parser::basicType() {
    JCPrimitiveTypeTree *t = new JCPrimitiveTypeTree(typeTag(L.token()));
    L.nextToken();
    return t;
}

int Parser::typeTag(Token &token) {
    switch (token.id) {
        case Token::INT:
            return TypeTags::INT;
        case Token::BOOLEAN:
            return TypeTags::BOOLEAN;
        default:
            return -1;
    }
}

JCExpression *Parser::bracketOpt(JCExpression *e) {
    if (L.token() == Token::LBRACKET) {
        L.nextToken();
        e = bracketsOptCont(e);
    }
    return e;
}

JCExpression *Parser::bracketsOptCont(JCExpression *e) {
    match(Token::RBRACKET);
    e = bracketOpt(e);
    return new JCArrayTypeTree(e);
}

vector<JCExpression *> *Parser::formalParameters() {
    vector<JCExpression *> *vec = new vector<JCExpression *>();
    match(Token::LPAREN);
    if (L.token() != Token::RPAREN) {
        vec.push_back(formalParameter());
        while (L.token() == Token::COMMA) {
            L.nextToken();
            vec.push_back(formalParameter());
        }
    }
    match(Token::RPAREN);
    return vec;
}

JCVariableDecl *Parser::formalParameter() {
    JCExpression *type = parseType();
    return new JCVariableDecl(type, ident());
}

JCBlock *Parser::block() {
    match(Token::LBRACE);
    vector<JCStatement *> *vec = blockStatements();
    match(Token::RBRACE);
    return new JCBlock(vec);
}

/**
 * LocalVariableDeclarationStatement
 * Statement
 */
vector<JCStatement *> *Parser::blockStatements() {
    vector<JCStatement *> *stats = new vector<JCStatement *>();
    while (true) {
        switch (L.token().id) {
            case Token::ID_RBRACE:
            case Token::ID_EOF:
                return stats;
            case Token::ID_IF:
            case Token::ID_FOR:
            case Token::ID_RETURN:
                stats->push_back(parseStatement());
                break;
            default:
                break;

        }
    }
    return stats;
}


JCStatement *Parser::parseStatement() {
    switch (L.token().id) {
        case Token::ID_LBRACE:
            return block();
        case Token::ID_IF: {
            L.nextToken();
            JCExpression *cond = parExpression();
            JCStatement *thenpart = parseStatement();
            JCStatement *elsepart = nullptr;
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
            vector<JCStatement *> *inits = forInit();
            match(Token::SEMI);
            JCExpression *cond = L.token() == Token::SEMI ? nullptr : term(EXPR);
            match(Token::SEMI);
            vector<JCExpressionStatement *> *steps = forUpdate();
            match(Token::RPAREN);

            JCStatement *body = parseStatement();

            return JCForLoop(inits, cond, steps, body);
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
            JCExpression *e = term(EXPR);
            match(Token::SEMI);
            return new JCReturn(e);
        }

        default:
            errno("not support " + L.token().fullDesc() + " in statement");
    }
    return nullptr;
}

JCExpression *Parser::parExpression() {
    match(Token::LPAREN);
    JCExpression *t = term(EXPR);
    match(Token::RPAREN);
    return t;
}

vector<JCStatement *> *Parser::forInit() {
    vector<JCStatement *> *stats = new vector<JCStatement *>();
    if (L.token() == Token::SEMI) {
        return stats;
    }

    //only support syntax like: for(int a = 0; ...) or for(a = 2; ...)
    //not for(int a=0, b=2)
    JCExpression *t = term(EXPR | TYPE);
    stats->push_back(t);
    return stats;
}


vector<JCExpressionStatement *> *Parser::forUpdate() {
    vector<JCExpressionStatement *> *stats = new vector<JCExpressionStatement *>();
    if (L.token() == Token::RPAREN) {
        return stats;
    }

    stats->push_back(term(EXPR));
    return stats;
}
