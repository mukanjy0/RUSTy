#include <iostream>
#include <string>
#include <list>

class Visitor;
class Stmt;

struct Var {
    enum Type { BOOL, CHAR, I32, STR, UNDEFINED};
    bool mut {};
    Type type {};
    int numericValue;
    std::string stringValue;

    Var() : type(UNDEFINED) {}
    Var(Type type, std::string stringValue)
        : type(type), stringValue(stringValue) {}
    Var(Type type, int numericValue)
        : type(type), numericValue(numericValue) {}
    Var(bool mut, Type type, int numericValue, std::string stringValue)
        : mut(mut), type(type), numericValue(numericValue),
        stringValue(std::move(stringValue)) {}
    ~Var() {}
    static Type stringToType(std::string type);
};

class Block {
    std::list<Stmt*> stmts;

public:
    Block(std::list<Stmt *> stmts) : stmts(std::move(stmts)) {}
    ~Block();
    Var accept(Visitor *visitor);
    friend std::ostream& operator<<(std::ostream& out, Block* block);
};

class Exp {
public:
    virtual ~Exp() = 0;
    virtual Var accept(Visitor* visitor);
    virtual void print(std::ostream& out) {}
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
    ~BinaryExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);

private:
    Operation op;
    Exp* lhs;
    Exp* rhs;
};

class UnaryExp : public Exp {
public:
    enum Operation {
        LNOT, // logical
    };

    UnaryExp(Operation op, Exp *exp) : op(op), exp(exp) {}
    ~UnaryExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);

private:
    Operation op;
    Exp* exp;
};

class Number : public Exp {
    int value;

public:
    Number(int value) : value(value) {}
    ~Number();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class Variable : public Exp {
    std::string name;

public:
    Variable(std::string name) : name(std::move(name)) {}
    ~Variable();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class FunCall : public Exp {
    std::string id;
    std::list<Exp*> args;

public:
    FunCall(std::string id, std::list<Exp*> args) : id(id), args(args) {}
    ~FunCall();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class IfBranch {
    Exp* cond;
    Block* block;
    friend class IfExp;
public:
    IfBranch(Exp* cond, Block* block) : cond(cond), block(block) {}
    ~IfBranch();
    friend std::ostream& operator<<(std::ostream& out, IfBranch* ifBranch);
};

class IfExp : public Exp {
    IfBranch* ifBranch;
    std::list<IfBranch*> elseIfBranches;
    IfBranch* elseBranch {};

public:
    IfExp(IfBranch* ifBranch)
        : ifBranch(ifBranch) {}
    IfExp(IfBranch* ifBranch, std::list<IfBranch*> elseIfBranches)
        : ifBranch(ifBranch), elseIfBranches(std::move(elseIfBranches)) {}
    IfExp(IfBranch* ifBranch, std::list<IfBranch*> elseIfBranches,
          IfBranch* elseBranch)
        : ifBranch(ifBranch), elseIfBranches(std::move(elseIfBranches)),
        elseBranch(elseBranch) {}
    ~IfExp();

    void setIfBranch(IfBranch* branch);
    void setElseBranch(IfBranch* branch);

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class LoopExp : public Exp {
    Block* block;

public:
    LoopExp(Block *block) : block(block) {}
    ~LoopExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

