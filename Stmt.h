#include "Exp.h"
#include <map>
#include <list>
#include <string>

class Block;

class Stmt {
public:
    virtual ~Stmt() = 0;
    virtual void accept(Visitor* visitor) = 0;
    virtual void print(std::ostream& out) {}
    friend std::ostream& operator<<(std::ostream& out, Exp* exp);
};

class DecStmt : public Stmt {
    std::string id;
    Var var;
    Exp* rhs {};

public:
    DecStmt(std::string id, Var var)
    : id(std::move(id)), var(var) {}

    DecStmt(std::string id, Var var, Exp *rhs)
    : id(std::move(id)), var(var), rhs(rhs) {}

    ~DecStmt();

    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class AssignStmt : public Stmt {
    std::string lhs;
    Exp* rhs;

public:
    AssignStmt(std::string lhs, Exp *rhs) : lhs(std::move(lhs)), rhs(rhs) {}
    ~AssignStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class ForStmt : public Stmt {
    std::string id;
    Exp* start;
    Exp* end;
    Block* block;
    bool inclusive {};

public:
    ForStmt(std::string id, Exp *start, Exp *end, Block *block)
    : id(std::move(id)), start(start), end(end), block(block) {}
    ForStmt(std::string id, Exp *start, Exp *end, Block *block, bool inclusive)
    : id(std::move(id)), start(start), end(end), block(block), inclusive(inclusive) {}
    ~ForStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class WhileStmt : public Stmt {
    Exp* cond;
    Block* block;

public:
    WhileStmt(Exp *cond, Block *block) : cond(cond), block(block) {}
    ~WhileStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class PrintStmt : public Stmt {
    std::string strLiteral;
    std::list<Exp*> args;

public:
    PrintStmt(std::string strLiteral, std::list<Exp *> args)
    : strLiteral(std::move(strLiteral)), args(std::move(args)) {}
    ~PrintStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class BreakStmt : public Stmt {
    Exp* exp {};
public:
    BreakStmt() = default;
    BreakStmt(Exp* exp) : exp(exp) {};
    ~BreakStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class ReturnStmt : public Stmt {
    Exp* exp {};
public:
    ReturnStmt() = default;
    ReturnStmt(Exp* exp) : exp(exp) {};
    ~ReturnStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class ExpStmt : public Stmt {
    Exp* exp {};
    bool returnValue {};
public:
    ExpStmt() = default;
    ExpStmt(Exp* exp) : exp(exp) {};
    ExpStmt(Exp* exp, bool returnValue) : exp(exp), returnValue(returnValue) {};
    ~ExpStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

