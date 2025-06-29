#include <string>
#include <list>

class Visitor;
class Stmt;

struct Var {
    enum Type { BOOL, CHAR, I32, STR };
    bool mut {};
    Type type;
    int numericValue;
    std::string stringValue;

    Var() : type(I32), numericValue(0) {}
    Var(Type type, std::string stringValue)
        : type(type), stringValue(stringValue) {}
    Var(Type type, int numericValue)
        : type(type), numericValue(numericValue) {}
    Var(bool mut, Type type, int numericValue, std::string stringValue)
        : mut(mut), type(type), numericValue(numericValue),
        stringValue(std::move(stringValue)) {}
    ~Var() {}
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

class IfBranch {
    Exp* cond;
    Block* block;
    friend class IfExp;
public:
    IfBranch(Exp* cond, Block* block) : cond(cond), block(block) {}
    ~IfBranch();
    friend std::ostream& operator<<(std::ostream& out, IfBranch* ifBranch);
};

class IfExp {
    IfBranch* ifBranch;
    std::list<IfBranch*> elseIfBranches;
    IfBranch* elseBranch;

public:
    IfExp(IfBranch* ifBranch, std::list<IfBranch*> elseIfBranches,
          IfBranch* elseBranch)
        : ifBranch(ifBranch), elseIfBranches(std::move(elseIfBranches)),
        elseBranch(elseBranch) {}
    ~IfExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class LoopExp {
    Block* block;

public:
    LoopExp(Block *block) : block(block) {}
    ~LoopExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class BinaryExp {
    enum Operation {
        LAND, LOR, LNOT, // logical
        GT, LT, GE, LE, EQ, NEQ, // relational
        PLUS, MINUS, TIMES, DIV, // arithmetical
    };

    Operation op;
    Exp* lhs;
    Exp* rhs;

public:
    BinaryExp(Operation op, Exp *lhs, Exp *rhs) : op(op), lhs(lhs), rhs(rhs) {}
    ~BinaryExp();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class Number {
    int value;

public:
    Number(int value) : value(value) {}
    ~Number();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class Variable {
    std::string name;

public:
    Variable(std::string name) : name(std::move(name)) {}
    ~Variable();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};

class FunCall {
    std::string id;
    std::list<Exp*> args;

public:
    FunCall(std::string id, std::list<Exp*> args) : id(id), args(args) {}
    ~FunCall();

    virtual void print(std::ostream& out);
    Var accept(Visitor* visitor);
};
