#ifndef VISITOR_H
#define VISITOR_H

#include "../syntax/Fun.h"

class Visitor {
public:
    virtual ~Visitor() = 0;
    virtual Var visit(Block* block) = 0;
    virtual Var visit(BinaryExp* exp) = 0;
    virtual Var visit(UnaryExp* exp) = 0;
    virtual Var visit(Number* exp) = 0;
    virtual Var visit(Variable* exp) = 0;
    virtual Var visit(FunCall* exp) = 0;
    virtual Var visit(IfExp* exp) = 0;
    virtual Var visit(LoopExp* exp) = 0;
    virtual void visit(DecStmt* stmt) = 0;
    virtual void visit(AssignStmt* stmt) = 0;
    virtual void visit(ForStmt* stmt) = 0;
    virtual void visit(WhileStmt* stmt) = 0;
    virtual void visit(PrintStmt* stmt) = 0;
    virtual void visit(BreakStmt* stmt) = 0;
    virtual void visit(ReturnStmt* stmt) = 0;
    virtual void visit(ExpStmt* stmt) = 0;
    virtual void visit(Fun* fun) = 0;
    virtual void visit(Program* program) = 0;
};

#endif
