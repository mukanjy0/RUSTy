#ifndef STMT_H
#define STMT_H

#include "Exp.h"
#include <list>
#include <string>

class DecStmt : public Stmt {
    std::string id;
    Value var;
    Exp* rhs {};

public:
    DecStmt(std::string id, Value var)
    : id(std::move(id)), var(var) {}

    DecStmt(std::string id, Value var, Exp *rhs)
    : id(std::move(id)), var(var), rhs(rhs) {}

    ~DecStmt();

    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class AssignStmt : public Stmt {
    Exp* lhs; // will be either Valueiable or Subscript
    Exp* rhs;
    bool ref {};

public:
    AssignStmt(Exp* lhs, Exp *rhs) 
        : lhs(std::move(lhs)), rhs(rhs) {}

    AssignStmt(Exp* lhs, Exp *rhs, bool ref) 
        : lhs(std::move(lhs)), rhs(rhs), ref(ref) {}

    ~AssignStmt();
    void print(std::ostream& out);
    void accept(Visitor* visitor);
};

class CompoundAssignStmt : public Stmt {
    BinaryExp::Operation op;
    Exp* lhs; // will be either Valueiable or Subscript
    Exp* rhs;

public:
    CompoundAssignStmt(BinaryExp::Operation op, Exp *lhs, Exp *rhs) 
        : op(op), lhs(lhs), rhs(rhs) {}

    ~CompoundAssignStmt();
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
    PrintStmt(std::string strLiteral)
    : strLiteral(std::move(strLiteral)) {}
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

#endif
