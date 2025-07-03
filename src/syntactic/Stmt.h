#ifndef STMT_H
#define STMT_H

#define FRIENDS friend class CodeGen; friend class TypeCheck; friend class NameRes;

#include "Exp.h"
#include <map> // used in parser
#include <list>
#include <string>

class DecStmt : public Stmt {
    FRIENDS
    std::string id;
    Val var;
    Exp* rhs {};

public:
    DecStmt(std::string id, const Val& var)
    : id(std::move(id)), var(var) {}

    DecStmt(std::string id, const Val& var, Exp *rhs)
    : id(std::move(id)), var(var), rhs(rhs) {}

    ~DecStmt() override;

    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

class AssignStmt : public Stmt {
    FRIENDS
    Exp *lhs; // will be either Variable or Subscript
    Exp* rhs;
    bool ref {};

public:
    AssignStmt(Exp* lhs, Exp *rhs)
        : lhs(lhs), rhs(rhs) {}

    AssignStmt(Exp* lhs, Exp *rhs, bool ref)
        : lhs(lhs), rhs(rhs), ref(ref) {}

    ~AssignStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

class CompoundAssignStmt : public Stmt {
    FRIENDS
    BinaryExp::Operation op;
    Exp* lhs; // will be either Variable or Subscript
    Exp* rhs;

public:
    CompoundAssignStmt(BinaryExp::Operation op, Exp *lhs, Exp *rhs)
        : op(op), lhs(lhs), rhs(rhs) {}

    ~CompoundAssignStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

class ForStmt : public Stmt {
    FRIENDS
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
};

class WhileStmt : public Stmt {
    FRIENDS
    Exp* cond;
    Block* block;

public:
    WhileStmt(Exp *cond, Block *block) : cond(cond), block(block) {}
    ~WhileStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

class PrintStmt : public Stmt {
    FRIENDS
    std::string strLiteral;
    std::list<Exp*> args;

public:
    explicit PrintStmt(std::string strLiteral)
    : strLiteral(std::move(strLiteral)) {}
    PrintStmt(std::string strLiteral, std::list<Exp *> args)
    : strLiteral(std::move(strLiteral)), args(std::move(args)) {}
    ~PrintStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

class BreakStmt : public Stmt {
    FRIENDS
    Exp* exp {};
public:
    BreakStmt() = default;
    explicit BreakStmt(Exp* exp) : exp(exp) {};
    ~BreakStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

class ReturnStmt : public Stmt {
    FRIENDS
    Exp* exp {};
public:
    ReturnStmt() = default;
    explicit ReturnStmt(Exp* exp) : exp(exp) {};
    ~ReturnStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

class ExpStmt : public Stmt {
    FRIENDS
    Exp* exp {};
    bool returnValue {};
public:
    ExpStmt() = default;
    explicit ExpStmt(Exp* exp) : exp(exp) {};
    ExpStmt(Exp* exp, bool returnValue) : exp(exp), returnValue(returnValue) {};
    ~ExpStmt() override;
    void print(std::ostream& out) override;
    void accept(Visitor* visitor) override;
};

#endif
