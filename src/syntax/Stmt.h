#ifndef STMT_H
#define STMT_H

#include "Exp.h"
#include <map>
#include <list>
#include <string>

#define FRIENDS friend class CodeGen; friend class TypeCheck; friend class NameRes;

class DecStmt : public Stmt {
    std::string id;
    Var var;
    Exp* rhs {};

public:
    DecStmt(std::string id, Var var)
    : id(std::move(id)), var(var) {}

    DecStmt(std::string id, Var var, Exp *rhs)
    : id(std::move(id)), var(var), rhs(rhs) {}

    ~DecStmt() override;

    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
};

class AssignStmt : public Stmt {
    std::string lhs;
    Exp* rhs;

public:
    AssignStmt(std::string lhs, Exp *rhs) : lhs(std::move(lhs)), rhs(rhs) {}
    ~AssignStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
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
    ~ForStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
};

class WhileStmt : public Stmt {
    Exp* cond;
    Block* block;

public:
    WhileStmt(Exp *cond, Block *block) : cond(cond), block(block) {}
    ~WhileStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
};

class PrintStmt : public Stmt {
    std::string strLiteral;
    std::list<Exp*> args;

public:
    PrintStmt(std::string strLiteral)
    : strLiteral(std::move(strLiteral)) {}
    PrintStmt(std::string strLiteral, std::list<Exp *> args)
    : strLiteral(std::move(strLiteral)), args(std::move(args)) {}
    ~PrintStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
};

class BreakStmt : public Stmt {
    Exp* exp {};
public:
    BreakStmt() = default;
    BreakStmt(Exp* exp) : exp(exp) {};
    ~BreakStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
};

class ReturnStmt : public Stmt {
    Exp* exp {};
public:
    ReturnStmt() = default;
    ReturnStmt(Exp* exp) : exp(exp) {};
    ~ReturnStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
};

class ExpStmt : public Stmt {
    Exp* exp {};
    bool returnValue {};
public:
    ExpStmt() = default;
    ExpStmt(Exp* exp) : exp(exp) {};
    ExpStmt(Exp* exp, bool returnValue) : exp(exp), returnValue(returnValue) {};
    ~ExpStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
    FRIENDS
};

#endif
