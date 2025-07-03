#ifndef EXP_H
#define EXP_H

#include <iostream>
#include <string>
#include <list>

class Visitor;
class CodeGen;

struct Var {
    enum Type { BOOL, CHAR, I32, STR, ID, UNIT, UNDEFINED};
    bool ref {};
    bool mut {};
    Type type {};
    // by default size = 0, otherwise, it's an array
    int size {};
    // for subscript expression
    int index {};
    // for slice expression
    int left, right;
    // in case of array has multiple values, otherwise only one
    std::list<int> numericValues; 
    // in case of array has multiple values, otherwise only one
    std::list<std::string> stringValues;

    Var() : type(UNDEFINED) {}

    Var(Type type, std::string stringValue, bool ref=false, bool mut=false)
        : type(type), stringValues({stringValue}),
        ref(ref), mut(mut) {}

    Var(Type type, int numericValue, bool ref=false, bool mut=false)
        : type(type), numericValues({numericValue}),
        ref(ref), mut(mut) {}

    Var(Type type, std::list<std::string> stringValues,
        bool ref=false, bool mut=false)
        : type(type), stringValues(std::move(stringValues)),
        ref(ref), mut(mut) {}

    Var(Type type, std::list<int> numericValues,
        bool ref=false, bool mut=false)
        : type(type), numericValues(std::move(numericValues)),
        ref(ref), mut(mut) {}

    ~Var() {}

    static Type stringToType(std::string type);
    friend std::ostream& operator<<(std::ostream& out, const Var& var);
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
    friend class CodeGen;

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
    ~BinaryExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
private:
    friend class CodeGen;

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
    friend class CodeGen;

    Operation op;
    Exp* exp;
};

class Literal : public Exp {
    friend class CodeGen;

    Var value;

public:
    explicit Literal(Var value) : value(value) {}
    ~Literal();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class Variable : public Exp {
    friend class CodeGen;

    std::string name;

public:
    Variable(std::string name) : name(std::move(name)) {}
    ~Variable();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class FunCall : public Exp {
  friend class CodeGen;

  std::string id;
  std::list<Exp *> args;

public:
    FunCall(std::string id, std::list<Exp *> args)
          : id(std::move(id)), args(std::move(args)) {}
    ~FunCall();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class IfBranch {
    friend class IfExp;

    Exp* cond;
    Block* block;
public:
    IfBranch(Exp *cond, Block *block) : cond(cond), block(block) {}
    ~IfBranch();

    IfBranch(const IfBranch &) = default;
    IfBranch(IfBranch &&) = delete;
    IfBranch &operator=(const IfBranch &) = default;
    IfBranch &operator=(IfBranch &&) = delete;

    friend std::ostream &operator<<(std::ostream &out, IfBranch ifBranch);
    friend std::ostream &operator<<(std::ostream &out, IfBranch *ifBranch);
};

class IfExp : public Exp {
    friend class CodeGen;

    IfBranch ifBranch;
    std::list<IfBranch> elseIfBranches;
    IfBranch* elseBranch {};

public:
    IfExp(const IfBranch &ifBranch, std::list<IfBranch> elseIfBranches)
        : ifBranch(ifBranch), elseIfBranches(std::move(elseIfBranches)) {}
    ~IfExp();

    IfExp(const IfExp &) = default;
    IfExp(IfExp &&) = delete;
    IfExp &operator=(const IfExp &) = default;
    IfExp &operator=(IfExp &&) = delete;

    void setIfBranch(const IfBranch& branch);
    void setElseBranch(IfBranch *branch);

    virtual void print(std::ostream &out);
    Var accept(Visitor *visitor);
};

class LoopExp : public Exp {
    friend class CodeGen;

    Block* block;
public:
    LoopExp(Block *block) : block(block) {}
    ~LoopExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class SubscriptExp : public Exp {
    friend class CodeGen;

    std::string id;
    Exp* exp;
public:
    SubscriptExp(std::string id, Exp* exp) : id(id), exp(exp) {}
    ~SubscriptExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class SliceExp : public Exp {
    friend class CodeGen;

    std::string id;
    Exp* start;
    Exp* end;
    bool inclusive {};
public:
    SliceExp(std::string id, Exp* start, Exp* end) 
        : id(id), start(start), end(end) {}
    SliceExp(std::string id, Exp* start, Exp* end, bool inclusive) 
        : id(id), start(start), end(end), inclusive(inclusive) {}
    ~SliceExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class ReferenceExp : public Exp {
    friend class CodeGen;

    Exp* exp;
    int count; // number of reference operators
public:
    ReferenceExp(Exp* exp)  
        : exp(exp), count(1) {}
    ReferenceExp(Exp* exp, int count)  
        : exp(exp), count(count) {}
    ~ReferenceExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class ArrayExp : public Exp {
    friend class CodeGen;

    std::list<Exp*> elements;
public:
    explicit ArrayExp(std::list<Exp *> elements)
            : elements(std::move(elements)) {}
    ~ArrayExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class UniformArrayExp : public Exp {
    friend class CodeGen;

    Exp *value;
    Exp *size;
public:
    UniformArrayExp(Exp *value, Exp *size) : value(value), size(size) {}
    ~UniformArrayExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

#endif
