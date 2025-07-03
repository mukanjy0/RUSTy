#ifndef VISITOR_H
#define VISITOR_H

#include "../syntactic/Fun.h"
#include "SymbolTable.h"

class Visitor {
protected:
    SymbolTable* table {};
public:
    explicit Visitor(SymbolTable* table = nullptr) : table(table) {}
    virtual ~Visitor() = 0;
    virtual Value visit(Block* block) = 0;
    virtual Value visit(BinaryExp* exp) = 0;
    virtual Value visit(UnaryExp* exp) = 0;
    virtual Value visit(Literal* exp) = 0;
    virtual Value visit(Variable* exp) = 0;
    virtual Value visit(FunCall* exp) = 0;
    virtual Value visit(IfExp* exp) = 0;
    virtual Value visit(LoopExp* exp) = 0;
    virtual Value visit(SubscriptExp* exp) = 0;
    virtual Value visit(SliceExp* exp) = 0;
    virtual Value visit(ReferenceExp* exp) = 0;
    virtual Value visit(ArrayExp* exp) = 0;
    virtual Value visit(UniformArrayExp* exp) = 0;
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

#endif
