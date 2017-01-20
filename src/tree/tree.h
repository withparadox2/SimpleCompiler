//
// Created by withparadox2 on 2016/11/14.
//

#ifndef SIMPLECOMPILER_TREE_H
#define SIMPLECOMPILER_TREE_H

#include <vector>
#include <memory>
#include <string>
#include "treevisitor.h"
#include "../parser/Token.h"
#include "alltree.h"
#include "../code/TypeTags.h"
#include "../code/symbols.h"
#include "../code/types.h"

using std::vector;
using std::shared_ptr;
using std::weak_ptr;
using std::move;
using std::string;

class Visitor;
class Name;

class Tree : public std::enable_shared_from_this<Tree> {
public:
    typedef shared_ptr<Tree> Ptr;
    typedef weak_ptr<Tree> WeakPtr;
    typedef vector<Ptr> List;

    int treeTag;

    Tree(int tag);

    TypePtr type;

    template<typename R, typename D>
    R accept(TreeVisitor<R, D> v, D& d);

    virtual void accept(Visitor* visitor) = 0;

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

class JCModifiers : public Tree {
public:
    typedef shared_ptr<JCModifiers> Ptr;

    long long flags;

    JCModifiers(long long flags);

    void accept(Visitor* visitor) override;
};

class JCClassDecl : public Tree {
public:
    typedef shared_ptr<JCClassDecl> Ptr;

    JCModifiers::Ptr mods;
    Name& name;
    Tree::List defs;
    ClassSymbolPtr sym;

    JCClassDecl(JCModifiers* mods, Name& name, Tree::List& defs);

    void accept(Visitor* visitor) override;
};

class JCExpression : public Tree {
public:
    typedef shared_ptr<JCExpression> Ptr;
    typedef vector<Ptr> List;

    JCExpression(int tag);
};

class JCStatement : public Tree {
public:
    typedef shared_ptr<JCStatement> Ptr;
    typedef vector<Ptr> List;

    JCStatement(int tag);
};


class JCExpressionStatement : public JCStatement {
public:
    typedef shared_ptr<JCExpressionStatement> Ptr;
    typedef vector<Ptr> List;

    JCExpression::Ptr exp;

    JCExpressionStatement(JCExpression* exp);

    void accept(Visitor* visitor) override;
};

class JCBlock : public JCStatement {
public:
    typedef shared_ptr<JCBlock> Ptr;

    JCStatement::List stats;

    JCBlock(JCStatement::List& stats);

    void accept(Visitor* visitor) override;
};

class JCForLoop : public JCStatement {
public:
    typedef shared_ptr<JCForLoop> Ptr;

    JCStatement::List init;
    JCExpression::Ptr cond;
    JCExpressionStatement::List step;
    JCStatement::Ptr body;

    JCForLoop(JCStatement::List& init,
              JCExpression* cond,
              JCExpressionStatement::List step,
              JCStatement* body);

    void accept(Visitor* visitor) override;
};

class JCIf : public JCStatement {
public:
    typedef shared_ptr<JCIf> Ptr;

    JCExpression::Ptr cond;
    JCStatement::Ptr thenPart;
    JCStatement::Ptr elsePart;

    JCIf(JCExpression* cond,
         JCStatement* thenpart,
         JCStatement* elsepart);

    void accept(Visitor* visitor) override;
};


class JCBreak : public JCStatement {
public:
    typedef shared_ptr<JCBreak> Ptr;
    //not support label

    //target point out to the enclosing tree,
    //use weak_ptr to avoid circular reference.
    Tree::WeakPtr target;
    JCBreak();

    void accept(Visitor* visitor) override;
};

class JCContinue : public JCStatement {
public:
    typedef shared_ptr<JCContinue> Ptr;

    //same as JCBreak
    Tree::WeakPtr target;

    JCContinue();

    void accept(Visitor* visitor) override;
};

class JCReturn : public JCStatement {
public:
    typedef shared_ptr<JCReturn> Ptr;

    JCExpression::Ptr expr;

    JCReturn(JCExpression* expr);

    void accept(Visitor* visitor) override;

};

class JCMethodInvocation : public JCExpression {
public:
    typedef shared_ptr<JCMethodInvocation> Ptr;

    JCExpression::List args;
    JCExpression::Ptr meth;

    JCMethodInvocation(JCExpression::List& args, JCExpression* meth);

    void accept(Visitor* visitor) override;
};

class JCNewClass : public JCExpression {
public:
    typedef shared_ptr<JCNewClass> Ptr;

    JCExpression::List arguments;
    JCExpression::Ptr clazz;

    JCNewClass(JCExpression* clazz, JCExpression::List& arguments);

    void accept(Visitor* visitor) override;
};

class JCParens : public JCExpression {
public:
    typedef shared_ptr<JCParens> Ptr;

    JCExpression::Ptr expr;

    JCParens(JCExpression* expr);

    void accept(Visitor* visitor) override;
};

class JCAssign : public JCExpression {
public:
    typedef shared_ptr<JCAssign> Ptr;

    JCExpression::Ptr lhs;
    JCExpression::Ptr rhs;

    JCAssign(JCExpression* lhs, JCExpression* rhs);

    void accept(Visitor* visitor) override;
};

class JCConditional : public JCExpression {
public :
    typedef shared_ptr<JCConditional> Ptr;

    JCExpression::Ptr cond;
    JCExpression::Ptr truepart;
    JCExpression::Ptr falsepart;

    JCConditional(JCExpression* cond, JCExpression* truepart, JCExpression* falsepart);

    void accept(Visitor* visitor) override;
};

class JCBinary : public JCExpression {
public:
    typedef shared_ptr<JCBinary> Ptr;

    int opcode;
    JCExpression::Ptr lhs;
    JCExpression::Ptr rhs;

    JCBinary(int opcode, JCExpression* lhs, JCExpression* rhs);

    void accept(Visitor* visitor) override;
};

class JCArrayAccess : public JCExpression {
public:
    typedef shared_ptr<JCArrayAccess> Ptr;

    JCExpression::Ptr indexed;
    JCExpression::Ptr index;

    JCArrayAccess(JCExpression* indexed, JCExpression* index);

    void accept(Visitor* visitor) override;
};

class JCFieldAccess : public JCExpression {
public:
    typedef shared_ptr<JCFieldAccess> Ptr;

    Name& selector;
    JCExpression::Ptr selected;

    JCFieldAccess(JCExpression* selected, Name& selector);

    void accept(Visitor* visitor) override;
};

class JCIdent : public JCExpression {
public:
    typedef shared_ptr<JCIdent> Ptr;
    SymbolPtr sym;
    Name& name;

    JCIdent(Name& name);

    void accept(Visitor* visitor) override;
};

class JCLiteral : public JCExpression {
private:

    class IValueHolder {
    };

    template<typename T>
    class ValueHolder : public IValueHolder {
    public://let outer class read value
        T value;

        ValueHolder(T value);
    };

    IValueHolder* value;
public:
    typedef shared_ptr<JCLiteral> Ptr;

    int typetag;

    template<typename R>
    JCLiteral(int typetag, R value);

    template<typename R>
    R getValue();

    void accept(Visitor* visitor) override;

    ~JCLiteral();
};

template<typename T>
JCLiteral::ValueHolder<T>::ValueHolder(T value) : value(value) {}

template<typename R>
JCLiteral::JCLiteral(int typetag, R value) : JCExpression(LITERAL), typetag(typetag), value(new ValueHolder<R>(value)) {
}

template<typename R>
R JCLiteral::getValue() {
    ValueHolder<R>* p = static_cast<ValueHolder<R>*>(value);
    return p->value;
}

class JCPrimitiveTypeTree : public JCExpression {
public:
    typedef shared_ptr<JCPrimitiveTypeTree> Ptr;

    int typetag;

    JCPrimitiveTypeTree(int tag);

    void accept(Visitor* visitor) override;
};

class JCArrayTypeTree : public JCExpression {
public:
    JCExpression::Ptr elementType;

    JCArrayTypeTree(JCExpression* elementType);

    void accept(Visitor* visitor) override;
};

class JCNewArray : public JCExpression {
public:
    JCExpression::Ptr elementType;
    JCExpression::List dimens;
    JCExpression::List elems;//init list

    JCNewArray(JCExpression* elementType, JCExpression::List& dimens, JCExpression::List& elems);

    void accept(Visitor* visitor) override;
};

class JCUnary : public JCExpression {
public:
    int opcode;
    JCExpression::Ptr arg;

    JCUnary(int opcode, JCExpression* arg);

    void accept(Visitor* visitor) override;
};

class JCVariableDecl : public JCStatement {
public:
    typedef shared_ptr<JCVariableDecl> Ptr;
    typedef vector<Ptr> List;

    Name& name;
    JCExpression::Ptr vartype;
    JCExpression::Ptr init;
    VarSymbolPtr sym;

    JCVariableDecl(Name& name, JCExpression* vartype);

    JCVariableDecl(Name& name, JCExpression* vartype, JCExpression* init);

    void accept(Visitor* visitor) override;
};


class JCMethodDecl : public Tree {
public:
    typedef shared_ptr<JCMethodDecl> Ptr;

    JCModifiers::Ptr mods;
    JCExpression::Ptr resType;
    Name& name;
    JCVariableDecl::List params;
    JCBlock::Ptr body;
    MethodSymbolPtr sym;

    JCMethodDecl(JCModifiers* mods,
                 JCExpression* type,
                 Name& name,
                 JCVariableDecl::List& params,
                 JCBlock* body);

    void accept(Visitor* visitor) override;

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

    int opTag(Token& token);

    int opPrec(int ot);

    string descByTag(int treeTag);

    bool hasConstructors(const Tree::List& defs);

    bool isConstructor(const Tree& tree);

    Name* name(Tree* tree);

    Name& operatorName(int optag);
}

#endif //SIMPLECOMPILER_TREE_H
