//
// Created by withparadox2 on 2016/11/14.
//

#ifndef SIMPLECOMPILER_TREE_H
#define SIMPLECOMPILER_TREE_H

#include <vector>
#include "treevisitor.h"
#include "../util/names.h"
#include "../parser/Token.h"
#include "alltree.h"

using std::vector;

class Visitor;

class Tree {
public:
    //Type* type;
    int treeTag;

//    virtual void accept(Visitor &v);

    Tree(int tag);

    template<typename R, typename D>
    R accept(TreeVisitor<R, D> v, D &d);

    static const int CLASSDEF = 1;

    /** Method definitions, of type MethodDef.
     */
    static const int METHODDEF = CLASSDEF + 1;

    /** Variable definitions, of type VarDef.
     */
    static const int VARDEF = METHODDEF + 1;

    /** The no-op statement ";", of type Skip
     */
    static const int SKIP = VARDEF + 1;

    /** Blocks, of type Block.
     */
    static const int BLOCK = SKIP + 1;

    /** For-loops, of type ForLoop.
     */
    static const int FORLOOP = SKIP + 1;

    /** Conditional expressions, of type Conditional.*/
    static const int CONDEXPR = FORLOOP + 1;

    /** Conditional statements, of type If.
     */
    static const int IF = CONDEXPR + 1;

    /** Expression statements, of type Exec.
     */
    static const int EXEC = IF + 1;

    /** Break statements, of type Break.
     */
    static const int BREAK = EXEC + 1;

    /** Continue statements, of type Continue.
     */
    static const int CONTINUE = BREAK + 1;

    /** Return statements, of type Return.
     */
    static const int RETURN = CONTINUE + 1;

    /** Method invocation expressions, of type Apply.
     */
    static const int APPLY = RETURN + 1;

    /** Class instance creation expressions, of type NewClass.
     */
    static const int NEWCLASS = APPLY + 1;

    /** Array creation expressions, of type NewArray.
     */
    static const int NEWARRAY = NEWCLASS + 1;

    /** Parenthesized subexpressions, of type Parens.
     */
    static const int PARENS = NEWARRAY + 1;

    /** Assignment expressions, of type Assign.
     */
    static const int ASSIGN = PARENS + 1;

    /** Indexed array expressions, of type Indexed.
     */
    static const int INDEXED = ASSIGN + 1;

    /** Selections, of type Select.
     */
    static const int SELECT = INDEXED + 1;

    /** Simple identifiers, of type Ident.
     */
    static const int IDENT = SELECT + 1;

    /** Literals, of type Literal.
     */
    static const int LITERAL = IDENT + 1;

    /** Basic type identifiers, of type TypeIdent.
     */
    static const int TYPEIDENT = LITERAL + 1;

    /** Array types, of type TypeArray.
     */
    static const int TYPEARRAY = TYPEIDENT + 1;
    /** metadata: Modifiers
     */
    static const int MODIFIERS = TYPEIDENT + 1;

    /** Unary operators, of type Unary.
     */
    static const int POS = MODIFIERS + 1;             // +
    static const int NEG = POS + 1;                   // -
    static const int NOT = NEG + 1;                   // !
    static const int COMPL = NOT + 1;                 // ~


    /** unary operator for null reference checks, only used internally.
     */
    static const int NULLCHK = COMPL + 1;

    /** Binary operators, of type Binary.
     */
    static const int OR = NULLCHK + 1;                // ||
    static const int AND = OR + 1;                    // &&
    static const int EQ = AND + 1;                 // ==
    static const int NE = EQ + 1;                     // !=
    static const int LT = NE + 1;                     // <
    static const int GT = LT + 1;                     // >
    static const int LE = GT + 1;                     // <=
    static const int GE = LE + 1;                     // >=
    static const int PLUS = GE + 1;                  // +
    static const int MINUS = PLUS + 1;                // -
    static const int MUL = MINUS + 1;                 // *
    static const int DIV = MUL + 1;                   // /
    static const int MOD = DIV + 1;                   // %
};

class JCClassDecl : public Tree {
public:
    JCModifiers *mods;
    Name &name;
    vector<Tree *> *defs;

    JCClassDecl(JCModifiers *mods, Name &name, vector<Tree *> *defs);
};

class JCExpression : public Tree {
public:
    JCExpression(int tag);
};

class JCStatement : public Tree {
public:
    JCStatement(int tag);
};

class JCMethodDecl : public Tree {
public:
    JCModifiers *mods;
    JCExpression *type;
    Name *name;
    vector<JCVariableDecl *> *params;
    JCBlock *body;

    JCMethodDecl(JCModifiers *mods,
                 JCExpression *type,
                 Name *name,
                 vector<JCVariableDecl *> *params,
                 JCBlock *body);

};

class JCBlock : public JCStatement {
public:
    vector<JCStatement *> *stats;

    JCBlock(vector<JCStatement *> *stats);

};

class JCForLoop : public JCStatement {
public:
    vector<JCStatement *> *init;
    JCExpression *cond;
    vector<JCExpressionStatement *> *step;
    JCStatement *body;

    JCForLoop(vector<JCStatement *> *init,
              JCExpression *cond,
              vector<JCExpressionStatement *> *step,
              JCStatement *body);
};

class JCIf : public JCStatement {
public:
    JCExpression *cond;
    JCStatement *thenPart;
    JCStatement *elsePart;

    JCIf(JCExpression *cond,
         JCStatement *thenpart,
         JCStatement *elsepart);

};

class JCExpressionStatement : public JCStatement {
public:
    JCExpression *exp;

    JCExpressionStatement(JCExpression *exp);

};

class JCBreak : public JCStatement {
public:
    JCBreak();
};

class JCContinue : public JCStatement {
public:
    JCContinue();
};

class JCReturn : public JCStatement {
public:
    JCExpression *expr;

    JCReturn(JCExpression *expr);
};

class JCMethodInvocation : public JCExpression {
public:
    vector<JCExpression *> *args;
    JCExpression *meth;

    JCMethodInvocation(vector<JCExpression *> *args, JCExpression *meth);
};

class JCNewClass : public JCExpression {
public:
    vector<JCExpression *> *arguments;
    JCExpression *clazz;

    JCNewClass(JCExpression *clazz, vector<JCExpression *> *arguments);

};

class JCParens : public JCExpression {
public:
    JCExpression *expr;

    JCParens(JCExpression *expr);
};

class JCAssign : public JCExpression {
public:
    JCExpression *lhs;
    JCExpression *rhs;

    JCAssign(JCExpression *lhs, JCExpression *rhs);
};

class JCConditional : public JCExpression {
public :
    JCExpression *cond;
    JCExpression *truepart;
    JCExpression *falsepart;

    JCConditional(JCExpression *cond, JCExpression *truepart, JCExpression *flasepart);
};

class JCBinary : public JCExpression {
public:
    int opcode;
    JCExpression *lhs;
    JCExpression *rhs;

    JCBinary(int opcode, JCExpression *lhs, JCExpression *rhs);
};

class JCArrayAccess : public JCExpression {
public:
    JCExpression *indexed;
    JCExpression *index;

    JCArrayAccess(JCExpression *indexed, JCExpression *index);
};

class JCFieldAccess : public JCExpression {
public:
    Name &selector;
    JCExpression *selected;

    JCFieldAccess(JCExpression *selected, Name &selector);
};

class JCIdent : public JCExpression {
public:
    Name &name;

    JCIdent(Name &name);
};

template<class R>
class JCLiteral : public JCExpression {
public:
    R value;
    int typetag;

    JCLiteral(int typetag, R value);
};

template<class R>
JCLiteral<R>::JCLiteral(int typetag, R value) : JCExpression(LITERAL), typetag(typetag), value(value) {
}

class JCPrimitiveTypeTree : public JCExpression {
public:
    int typetag;

    JCPrimitiveTypeTree(int tag);
};

class JCArrayTypeTree : public JCExpression {
public:
    JCExpression *elementType;

    JCArrayTypeTree(JCExpression *elementType);
};

//todo add to visitor
class JCNewArray : public JCExpression {
public:
    JCExpression *elementType;
    vector<JCExpression *> *dimens;
    vector<JCExpression *> *elems;//init list

    JCNewArray(JCExpression *elementType, vector<JCExpression *> *dimens, vector<JCExpression *> *elems);
};

//todo add to visitor
class JCUnary : public JCExpression {
public:
    int opcode;
    JCExpression *arg;
    JCUnary(int opcode, JCExpression *arg);
};
class JCVariableDecl : public JCStatement {
public:
    Name &name;
    JCExpression *vartype;
    JCExpression *init;
    JCVariableDecl(Name &name, JCExpression *vartype);
    JCVariableDecl(Name &name, JCExpression *vartype, JCExpression *init);
};

class JCModifiers : public Tree {
public:
    int flags;

    JCModifiers(int flags);
};

class Visitor {
public:
    virtual void visitClassDef(JCClassDecl &that) { visitTree(that); }

    virtual void visitMethodDef(JCMethodDecl &that) { visitTree(that); }

    virtual void visitVarDef(JCVariableDecl &that) { visitTree(that); }

    virtual void visitBlock(JCBlock &that) { visitTree(that); }

    virtual void visitForLoop(JCForLoop &that) { visitTree(that); }

    virtual void visitIf(JCIf &that) { visitTree(that); }

    virtual void visitExec(JCExpressionStatement &that) { visitTree(that); }

    virtual void visitBreak(JCBreak &that) { visitTree(that); }

    virtual void visitContinue(JCContinue &that) { visitTree(that); }

    virtual void visitReturn(JCReturn &that) { visitTree(that); }

    virtual void visitApply(JCMethodInvocation &that) { visitTree(that); }

    virtual void visitNewClass(JCNewClass &that) { visitTree(that); }

    virtual void visitParens(JCParens &that) { visitTree(that); }

    virtual void visitAssign(JCAssign &that) { visitTree(that); }

    virtual void visitConditional(JCConditional &that) { visitTree(that); }

    virtual void visitBinary(JCBinary &that) { visitTree(that); }

    virtual void visitIndexed(JCArrayAccess &that) { visitTree(that); }

    virtual void visitSelect(JCFieldAccess &that) { visitTree(that); }

    virtual void visitIdent(JCIdent &that) { visitTree(that); }

    template<class T>
    virtual void visitLiteral(JCLiteral<T> &that) { visitTree(that); }

    virtual void visitTypeIdent(JCPrimitiveTypeTree &that) { visitTree(that); }

    virtual void visitTypeArray(JCArrayTypeTree &that) { visitTree(that); }

    virtual void visitModifiers(JCModifiers &that) { visitTree(that); }

    void visitTree(Tree &that) {
        //TODO error
    }
};


namespace treeinfo {
    const int notExpression = -1;   // not an expression
    const int noPrec = 0;           // no enclosing expression
    const int assignPrec = 1;
    const int assignopPrec = 2;
    const int condPrec = 3;
    const int orPrec = 4;
    const int andPrec = 5;
    const int bitorPrec = 6;
    const int bitxorPrec = 7;
    const int bitandPrec = 8;
    const int eqPrec = 9;
    const int ordPrec = 10;
    const int shiftPrec = 11;
    const int addPrec = 12;
    const int mulPrec = 13;
    const int prefixPrec = 14;
    const int postfixPrec = 15;
    const int precCount = 16;

    int opTag(Token &token);

    int opPrec(int ot);

}

#endif //SIMPLECOMPILER_TREE_H
