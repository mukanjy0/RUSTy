#ifndef EXP_H
#define EXP_H

#define FRIENDS friend class CodeGen; friend class TypeCheck; friend class NameRes;

#include <iostream>
#include <string>
#include <list>
#include <utility>

class Visitor;
class CodeGen;

struct Value {
    enum Type { UNDEFINED, BOOL, CHAR, I32, STR, ID, UNIT};

    Type type {};
    // in case of array has multiple values, otherwise only one
    std::list<int> numericValues {};
    // in case of array has multiple values, otherwise only one
    std::list<std::string> stringValues {};
    // array of types for functions
    std::list<Type> types {};

    bool fun {};
    bool ref {};
    bool mut {};
    bool initialized {};
    // by default size = 0, otherwise, it's an array
    int size {};
    // for subscript expression
    int index {};
    // for slice expression
    int left {}, right {};

    Value() : type(UNDEFINED), initialized(false) {}

    Value(Type type, std::string stringValue,
          bool ref=false, bool mut=false, bool fun=false)
        : type(type), stringValues({std::move(stringValue)}),
        ref(ref), mut(mut), initialized(true) {}

    Value(Type type, int numericValue,
          bool ref=false, bool mut=false, bool fun=false)
        : type(type), numericValues({numericValue}),
        ref(ref), mut(mut), initialized(true) {}

    Value(Type type, std::list<std::string> stringValues,
        bool ref=false, bool mut=false, bool fun=false)
        : type(type), stringValues(std::move(stringValues)),
        ref(ref), mut(mut), initialized(true) {}

    Value(Type type, std::list<int> numericValues,
        bool ref=false, bool mut=false, bool fun=false)
        : type(type), numericValues(std::move(numericValues)),
        ref(ref), mut(mut), initialized(true) {}

    Value(Type type, bool fun=false)
        : type(type), fun(fun), initialized(true) {}

    ~Value() = default;

    bool isArray();
    bool isFunction();
    void addType(Type type);
    std::string getId() const;

    static Type stringToType(std::string type);
    friend std::ostream& operator<<(std::ostream& out, const Value::Type& type);
    friend std::ostream& operator<<(std::ostream& out, const Value& var);
};

class Exp;

class Stmt {
    FRIENDS
protected:
    int line;
    int col;
public:
    Stmt(const Stmt &) = default;
    Stmt(Stmt &&) = delete;
    Stmt &operator=(const Stmt &) = default;
    Stmt &operator=(Stmt &&) = delete;
    Stmt(int line, int col) : line(line), col(col) {}
    virtual ~Stmt() = 0;
    virtual Value accept(Visitor *visitor) = 0;
    virtual void print(std::ostream &out) = 0;
    friend std::ostream &operator<<(std::ostream &out, Stmt *stmt);
};

class Block {
    FRIENDS

    int line;
    int col;
    std::list<Stmt *> stmts;
    Value::Type type{};
public:
    Block(int line, int col, std::list<Stmt *> stmts)
        : line(line), col(col), stmts(std::move(stmts)) {}
    ~Block();

    Block(const Block &) = default;
    Block(Block &&) = delete;
    Block &operator=(const Block &) = default;
    Block &operator=(Block &&) = delete;

    Value accept(Visitor *visitor);
    friend std::ostream& operator<<(std::ostream& out, Block* block);
};

class Exp {
    FRIENDS
protected:
    int line;
    int col;
    Value::Type type{};
public:
    Exp(int line, int col) : line(line), col(col) {}
    virtual ~Exp() = 0;
    virtual Value accept(Visitor *visitor) = 0;
    virtual void print(std::ostream &out) = 0;
    friend std::ostream &operator<<(std::ostream &out, Exp *exp);
};

class BinaryExp : public Exp {
public:
    enum Operation {
        LAND, LOR, // logical
        GT, LT, GE, LE, EQ, NEQ, // relational
        PLUS, MINUS, TIMES, DIV, // arithmetical
    };

    BinaryExp(int line, int col, Operation op, Exp *lhs, Exp *rhs) 
        : Exp(line, col), op(op), lhs(lhs), rhs(rhs) {}
    ~BinaryExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
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

    UnaryExp(int line, int col, Operation op, Exp *exp) 
        : Exp(line, col), op(op), exp(exp) {}
    ~UnaryExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);

private:
    FRIENDS

    Operation op;
    Exp* exp;
};

class Literal : public Exp {
    FRIENDS

    Value value;

public:
    Literal(int line, int col, Value value) 
        : Exp(line, col), value(value) {}
    ~Literal();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class Variable : public Exp {
    FRIENDS

    std::string name;

public:
    Variable(int line, int col, std::string name) 
        : Exp(line, col), name(std::move(name)) {}
    ~Variable();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class FunCall : public Exp {
    FRIENDS

  std::string id;
  std::list<Exp *> args;

public:
    FunCall(int line, int col, std::string id, std::list<Exp *> args)
          : Exp(line, col), id(std::move(id)), args(std::move(args)) {}
    ~FunCall();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class IfExp : public Exp {
public:
    class IfBranch {
        friend class IfExp;

        Exp* cond;
        Block* block;
        IfBranch(Exp *cond, Block *block) : cond(cond), block(block) {}

    public:
        ~IfBranch();
        Exp* getCondition();
        Block* getBlock();
        void print(std::ostream &out);
    };

    IfExp(int line, int col, Exp* cond, Block* block)
        : Exp(line, col)
    {
        ifBranch = new IfBranch(cond, block);
    }
    ~IfExp();

    IfExp(const IfExp &) = default;
    IfExp(IfExp &&) = delete;
    IfExp &operator=(const IfExp &) = default;
    IfExp &operator=(IfExp &&) = delete;

    void addElseIfBranch(Exp* cond, Block* block);
    void setElseBranch(Exp* cond, Block* block);

    IfBranch* getIfBranch();
    std::list<IfBranch*> getElseIfBranches();
    IfBranch* getElseBranch();

    void print(std::ostream &out);
    Value accept(Visitor *visitor);

private:
    FRIENDS

    IfBranch* ifBranch;
    std::list<IfBranch*> elseIfBranches;
    IfBranch* elseBranch {};

};

class LoopExp : public Exp {
    FRIENDS

    Block* block;
public:
    LoopExp(int line, int col, Block *block) 
        : Exp(line, col), block(block) {}
    ~LoopExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class SubscriptExp : public Exp {
    FRIENDS

    std::string id;
    Exp* exp;
public:
    SubscriptExp(int line, int col, std::string id, Exp* exp) 
        : Exp(line, col), id(id), exp(exp) {}
    ~SubscriptExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class SliceExp : public Exp {
    FRIENDS

    std::string id;
    Exp* start;
    Exp* end;
    bool inclusive {};
public:
    SliceExp(int line, int col, std::string id, Exp* start, Exp* end) 
        : Exp(line, col), id(id), start(start), end(end) {}
    SliceExp(int line, int col, std::string id, Exp* start, Exp* end, bool inclusive) 
        : Exp(line, col), id(id), start(start), end(end), inclusive(inclusive) {}
    ~SliceExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class ReferenceExp : public Exp {
    FRIENDS

    Exp* exp;
    int count; // number of reference operators
public:
    ReferenceExp(int line, int col, Exp* exp)  
        : Exp(line, col), exp(exp), count(1) {}
    ReferenceExp(int line, int col, Exp* exp, int count)  
        : Exp(line, col), exp(exp), count(count) {}
    ~ReferenceExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class ArrayExp : public Exp {
    FRIENDS

    std::list<Exp*> elements;
public:
    explicit ArrayExp(int line, int col, std::list<Exp *> elements)
            : Exp(line, col), elements(std::move(elements)) {}
    ~ArrayExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

class UniformArrayExp : public Exp {
    FRIENDS

    Exp *value;
    Exp *size;
public:
    UniformArrayExp(int line, int col, Exp *value, Exp *size) 
        : Exp(line, col), value(value), size(size) {}
    ~UniformArrayExp();

    void print(std::ostream& out);
    Value accept(Visitor* visitor);
};

#endif
