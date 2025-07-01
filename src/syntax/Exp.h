#ifndef EXP_H
#define EXP_H

#define FRIENDS friend class CodeGen; friend class TypeCheck;

#include <iostream>
#include <string>
#include <list>
#include <utility>

class Visitor;
class CodeGen;
class TypeCheck;

struct Var {
    enum Type { BOOL, CHAR, I32, STR, VOID, UNDEFINED};
    bool mut {};
    Type type {};
    int numericValue{};
    std::string stringValue;

    Var() : type(UNDEFINED) {}
    Var(Type type, std::string stringValue)
        : type(type), stringValue(std::move(stringValue)) {}
    Var(Type type, int numericValue)
        : type(type), numericValue(numericValue) {}
    Var(bool mut, Type type, int numericValue, std::string stringValue)
        : mut(mut), type(type), numericValue(numericValue),
        stringValue(std::move(stringValue)) {}
    ~Var() = default;
    static Type stringToType(std::string type);
};

class Exp;

class Stmt {
public:
    virtual ~Stmt() = 0;
    virtual void accept(Visitor* visitor) = 0;
    virtual void print(std::ostream& out) = 0;
    friend std::ostream& operator<<(std::ostream& out, Stmt* stmt);
};

class Block {
    std::list<Stmt*> stmts;
public:
    explicit Block(std::list<Stmt *> stmts) : stmts(std::move(stmts)) {}
    ~Block();
    Var accept(Visitor *visitor);
    friend std::ostream& operator<<(std::ostream& out, Block* block);
    FRIENDS
};

class Exp {
public:
    virtual ~Exp() = 0;
    virtual Var accept(Visitor* visitor) = 0;
    virtual void print(std::ostream& out) = 0;
    friend std::ostream& operator<<(std::ostream& out, Exp* exp);
};

class BinaryExp : public Exp {
public:
    enum Operation {
        LAND, LOR, // logical
        GT, LT, GE, LE, EQ, NEQ, // relational
        PLUS, MINUS, TIMES, DIV, // arithmetical
    };

    BinaryExp(Operation op, Exp *lhs, Exp *rhs) : op(op), lhs(lhs), rhs(rhs) {}
    ~BinaryExp() override;

    void print(std::ostream& out) override;
    Var accept(Visitor* visitor) override;
private:
    Operation op;
    Exp* lhs;
    Exp* rhs;
    FRIENDS
};

class UnaryExp : public Exp {
public:
    enum Operation {
        LNOT, // logical
    };

    UnaryExp(Operation op, Exp *exp) : op(op), exp(exp) {}
    ~UnaryExp() override;

    void print(std::ostream& out) override;
    Var accept(Visitor* visitor) override;

private:
    Operation op;
    Exp* exp;
    FRIENDS
};

class Number : public Exp {
    int value;

public:
    explicit Number(int value) : value(value) {}
    ~Number() override;

    void print(std::ostream& out) override;
    Var accept(Visitor* visitor) override;
    FRIENDS
};

class Variable : public Exp {
    std::string name;

public:
    explicit Variable(std::string name) : name(std::move(name)) {}
    ~Variable() override;

    void print(std::ostream& out) override;
    Var accept(Visitor* visitor) override;
    FRIENDS
};

class FunCall : public Exp {
    std::string id;
    std::list<Exp*> args;

public:
    FunCall(std::string id, std::list<Exp*> args) : id(std::move(id)), args(std::move(args)) {}
    ~FunCall() override;

    void print(std::ostream& out) override;
    Var accept(Visitor* visitor) override;
    FRIENDS
};

class IfBranch {
    Exp* cond;
    Block* block;
    friend class IfExp;
public:
    IfBranch(Exp* cond, Block* block) : cond(cond), block(block) {}
    ~IfBranch();
    friend std::ostream& operator<<(std::ostream& out, IfBranch ifBranch);
    friend std::ostream& operator<<(std::ostream& out, const IfBranch* ifBranch);
    FRIENDS
};

class IfExp : public Exp {
    IfBranch ifBranch;
    std::list<IfBranch> elseIfBranches;
    IfBranch* elseBranch {};

public:
    explicit IfExp(const IfBranch& ifBranch)
        : ifBranch(ifBranch) {}
    IfExp(const IfBranch& ifBranch, std::list<IfBranch> elseIfBranches)
        : ifBranch(ifBranch), elseIfBranches(std::move(elseIfBranches)) {}
    IfExp(const IfBranch& ifBranch, std::list<IfBranch> elseIfBranches,
          IfBranch* elseBranch)
        : ifBranch(ifBranch), elseIfBranches(std::move(elseIfBranches)),
        elseBranch(elseBranch) {}
    ~IfExp() override;

    void setIfBranch(IfBranch branch);
    void setElseBranch(IfBranch* branch);

    void print(std::ostream& out) override;
    Var accept(Visitor* visitor) override;
    FRIENDS
};

class LoopExp : public Exp {
    Block* block;

public:
    explicit LoopExp(Block *block) : block(block) {}
    ~LoopExp() override;

    void print(std::ostream& out) override;
    Var accept(Visitor* visitor) override;
    FRIENDS
};

#endif
