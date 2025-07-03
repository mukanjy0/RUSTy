#ifndef EXP_H
#define EXP_H

#define FRIENDS friend class CodeGen; friend class TypeCheck; friend class NameRes;

#include <iostream>
#include <string>
#include <list>
#include <utility>

class Visitor;

struct Val {
    enum Type { BOOL, CHAR, I32, STR, ID, UNIT, UNDEFINED};
    bool ref {};
    bool mut {};
    Type type {};
    // by default size = 0, otherwise, it's an array
    int size {};
    // for subscript expression
    int index {};
    // for slice expression
    int left{}, right{};
    // in case of array has multiple values, otherwise only one
    std::list<int> numericValues;
    // in case of array has multiple values, otherwise only one
    std::list<std::string> stringValues;

    Val() : type(UNDEFINED) {}

    Val(Type type, std::string stringValue, bool ref=false, bool mut=false)
        : type(type), stringValues({std::move(stringValue)}),
        ref(ref), mut(mut) {}

    Val(Type type, int numericValue, bool ref=false, bool mut=false)
        : type(type), numericValues({numericValue}),
        ref(ref), mut(mut) {}

    Val(Type type, std::list<std::string> stringValues,
        bool ref=false, bool mut=false)
        : type(type), stringValues(std::move(stringValues)),
        ref(ref), mut(mut) {}

    Val(Type type, std::list<int> numericValues,
        bool ref=false, bool mut=false)
        : type(type), numericValues(std::move(numericValues)),
        ref(ref), mut(mut) {}

    ~Val() = default;

    static Type stringToType(const std::string& type);
    friend std::ostream& operator<<(std::ostream& out, const Val& var);
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
    FRIENDS

    std::list<Stmt*> stmts;
public:
    explicit Block(std::list<Stmt *> stmts) : stmts(std::move(stmts)) {}
    ~Block();
    Val accept(Visitor *visitor);
    friend std::ostream& operator<<(std::ostream& out, const Block* block);
};

class Exp {
public:
    virtual ~Exp() = 0;
    virtual Val accept(Visitor* visitor) = 0;
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
    Val accept(Visitor* visitor) override;
private:
    FRIENDS

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
    ~UnaryExp() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;

private:
    FRIENDS

    Operation op;
    Exp* exp;
};

class Literal : public Exp {
    FRIENDS

    Val value;

public:
    explicit Literal(const Val& value) : value(value) {}
    ~Literal() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

class Variable : public Exp {
    FRIENDS

    std::string name;

public:
    explicit Variable(std::string name) : name(std::move(name)) {}
    ~Variable() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

class FunCall : public Exp {
    FRIENDS

    std::string id;
    std::list<Exp*> args;

public:
    FunCall(std::string id, std::list<Exp*> args) : id(std::move(id)), args(std::move(args)) {}
    ~FunCall() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

class IfBranch {
    friend class IfExp; FRIENDS

    Exp* cond;
    Block* block;
public:
    IfBranch(Exp* cond, Block* block) : cond(cond), block(block) {}
    ~IfBranch();
    friend std::ostream& operator<<(std::ostream& out, const IfBranch& ifBranch);
    friend std::ostream& operator<<(std::ostream& out, const IfBranch* ifBranch);
};

class IfExp : public Exp {
    FRIENDS

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

    void setIfBranch(const IfBranch& branch);
    void setElseBranch(IfBranch* branch);

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

class LoopExp : public Exp {
    FRIENDS

    Block* block;
public:
    explicit LoopExp(Block *block) : block(block) {}
    ~LoopExp() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

class SubscriptExp : public Exp {
    FRIENDS

    std::string id;
    Exp* exp;
public:
    SubscriptExp(std::string id, Exp* exp) : id(std::move(id)), exp(exp) {}
    ~SubscriptExp() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

class SliceExp : public Exp {
    FRIENDS

    std::string id;
    Exp* start;
    Exp* end;
    bool inclusive {};
public:
    SliceExp(std::string id, Exp* start, Exp* end)
        : id(std::move(id)), start(start), end(end) {}
    SliceExp(std::string id, Exp* start, Exp* end, bool inclusive)
        : id(std::move(id)), start(start), end(end), inclusive(inclusive) {}
    ~SliceExp() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

class ReferenceExp : public Exp {
    FRIENDS

    Exp* exp;
    int count; // number of reference operators
public:
    explicit ReferenceExp(Exp* exp)
        : exp(exp), count(1) {}
    ReferenceExp(Exp* exp, int count)
        : exp(exp), count(count) {}
    ~ReferenceExp() override;

    void print(std::ostream& out) override;
    Val accept(Visitor* visitor) override;
};

#endif
