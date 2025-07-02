#ifndef VISITOR_H
#define VISITOR_H

#include "Fun.h"

class Visitor {
public:
    virtual ~Visitor() = 0;
    virtual Var visit(Block* block) = 0;
    virtual Var visit(BinaryExp* exp) = 0;
    virtual Var visit(UnaryExp* exp) = 0;
    virtual Var visit(Literal* exp) = 0;
    virtual Var visit(Variable* exp) = 0;
    virtual Var visit(FunCall* exp) = 0;
    virtual Var visit(IfExp* exp) = 0;
    virtual Var visit(LoopExp* exp) = 0;
    virtual Var visit(SubscriptExp* exp) = 0;
    virtual Var visit(SliceExp* exp) = 0;
    virtual Var visit(ReferenceExp* exp) = 0;
    virtual void visit(DecStmt* stmt) = 0;
    virtual void visit(AssignStmt* stmt) = 0;
    virtual void visit(CompoundAssignStmt* stmt) = 0;
    virtual void visit(ForStmt* stmt) = 0;
    virtual void visit(WhileStmt* stmt) = 0;
    virtual void visit(PrintStmt* stmt) = 0;
    virtual void visit(BreakStmt* stmt) = 0;
    virtual void visit(ReturnStmt* stmt) = 0;
    virtual void visit(ExpStmt* stmt) = 0;
    virtual void visit(Fun* fun) = 0;
    virtual void visit(Program* program) = 0;
};

class Printer : public Visitor {
public:
    Printer() = default;
    ~Printer();
    Var visit(Block* block);
    Var visit(BinaryExp* exp);
    Var visit(UnaryExp* exp);
    Var visit(Literal* exp);
    Var visit(Variable* exp);
    Var visit(FunCall* exp);
    Var visit(IfExp* exp);
    Var visit(LoopExp* exp);
    Var visit(SubscriptExp* exp);
    Var visit(SliceExp* exp);
    Var visit(ReferenceExp* exp);
    void visit(DecStmt* stmt);
    void visit(AssignStmt* stmt);
    void visit(CompoundAssignStmt* stmt);
    void visit(ForStmt* stmt);
    void visit(WhileStmt* stmt);
    void visit(PrintStmt* stmt);
    void visit(BreakStmt* stmt);
    void visit(ReturnStmt* stmt);
    void visit(ExpStmt* stmt);
    void visit(Fun* fun);
    void visit(Program* program);
};

// class TypeChecker : public Visitor {
// public:
//     ~TypeChecker();
//     Var visit(Block* block);
//     Var visit(BinaryExp* exp);
//     Var visit(UnaryExp* exp);
//     Var visit(Literal* exp);
//     Var visit(Variable* exp);
//     Var visit(FunCall* exp);
//     Var visit(IfExp* exp);
//     Var visit(LoopExp* exp);
//     void visit(DecStmt* stmt);
//     void visit(AssignStmt* stmt);
//     void visit(ForStmt* stmt);
//     void visit(WhileStmt* stmt);
//     void visit(PrintStmt* stmt);
//     void visit(BreakStmt* stmt);
//     void visit(ReturnStmt* stmt);
//     void visit(ExpStmt* stmt);
//     void visit(Fun* fun);
//     void visit(Program* program);
// };



#endif
