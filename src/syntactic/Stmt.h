#ifndef STMT_H
#define STMT_H

#define FRIENDS friend class CodeGen; friend class TypeCheck; friend class NameRes;

#include "Exp.h"
#include <list>
#include <string>

class DecStmt : public Stmt {
    FRIENDS
    std::string id;
    Value var;
    Exp* rhs {};

public:
    DecStmt(int line, int col, std::string id, Value var)
        : Stmt(line, col), id(std::move(id)), var(var) {}

    DecStmt(int line, int col, std::string id, Value var, Exp *rhs)
        : Stmt(line, col), id(std::move(id)), var(var), rhs(rhs) {}

    ~DecStmt() override;

    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class AssignStmt : public Stmt {
    FRIENDS
    Exp *lhs; // will be either Variable or Subscript
    Exp* rhs;
    bool ref {};

public:
    AssignStmt(int line, int col, Exp* lhs, Exp *rhs)
        : Stmt(line, col), lhs(lhs), rhs(rhs) {}

    AssignStmt(int line, int col, Exp* lhs, Exp *rhs, bool ref)
        : Stmt(line, col), lhs(lhs), rhs(rhs), ref(ref) {}

    ~AssignStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class CompoundAssignStmt : public Stmt {
    FRIENDS
    BinaryExp::Operation op;
    Exp* lhs; // will be either Variable or Subscript
    Exp* rhs;

public:
    CompoundAssignStmt(int line, int col, BinaryExp::Operation op, Exp *lhs, Exp *rhs)
        : Stmt(line, col), op(op), lhs(lhs), rhs(rhs) {}

    ~CompoundAssignStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class ForStmt : public Stmt {
    FRIENDS
    std::string id;
    Exp* start;
    Exp* end;
    Block* block;
    bool inclusive {};

public:
    ForStmt(int line, int col,
            std::string id, Exp *start, Exp *end, Block *block)
    : Stmt(line, col), id(std::move(id)), start(start), end(end), block(block) {}

    ForStmt(int line, int col,
            std::string id, Exp *start, Exp *end, Block *block, bool inclusive)
    : Stmt(line, col), id(std::move(id)), start(start), end(end),
        block(block), inclusive(inclusive) {}
    ~ForStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class WhileStmt : public Stmt {
    FRIENDS
    Exp* cond;
    Block* block;

public:
    WhileStmt(int line, int col, Exp *cond, Block *block) 
        : Stmt(line, col), cond(cond), block(block) {}
    ~WhileStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class PrintStmt : public Stmt {
    FRIENDS
    std::string strLiteral;
    std::list<Exp*> args;

public:
    PrintStmt(int line, int col, std::string strLiteral)
    : Stmt(line, col), strLiteral(std::move(strLiteral)) {}
    PrintStmt(int line, int col, std::string strLiteral, std::list<Exp *> args)
    : Stmt(line, col), strLiteral(std::move(strLiteral)), args(std::move(args)) {}
    ~PrintStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class BreakStmt : public Stmt {
    FRIENDS
    Exp* exp {};
    Value::Type type{};
public:
    BreakStmt(int line, int col) : Stmt(line, col) {}
    BreakStmt(int line, int col, Exp* exp) 
        : Stmt(line, col), exp(exp) {};
    ~BreakStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class ReturnStmt : public Stmt {
    FRIENDS
    Exp* exp {};
    Value::Type type{};
public:
    ReturnStmt(int line, int col) : Stmt(line, col) {}
    ReturnStmt(int line, int col, Exp* exp) 
        : Stmt(line, col), exp(exp) {};
    ~ReturnStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

class ExpStmt : public Stmt {
    FRIENDS
    Exp* exp {};
    bool returnValue {};
    Value::Type type{};
public:
    ExpStmt(int line, int col) : Stmt(line, col) {}
    ExpStmt(int line, int col, Exp* exp) 
        : Stmt(line, col), exp(exp) {};
    ExpStmt(int line, int col, Exp* exp, bool returnValue) 
        : Stmt(line, col), exp(exp), returnValue(returnValue) {};
    ~ExpStmt() override;
    void print(std::ostream& out) override;
    Value accept(Visitor* visitor) override;
};

#endif
