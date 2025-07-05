#ifndef VISITOR_H
#define VISITOR_H

#include "../syntactic/Fun.h"
#include "SymbolTable.h"

class Visitor {
protected:
    SymbolTable* table {};
public:
    Visitor() {
        table = new SymbolTable();
    }
    explicit Visitor(SymbolTable* table) {
        if (table) this->table = table; else this->table = new SymbolTable();
    }
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
    virtual Value visit(DecStmt* stmt) = 0;
    virtual Value visit(AssignStmt* stmt) = 0;
    virtual Value visit(CompoundAssignStmt* stmt) = 0;
    virtual Value visit(ForStmt* stmt) = 0;
    virtual Value visit(WhileStmt* stmt) = 0;
    virtual Value visit(PrintStmt* stmt) = 0;
    virtual Value visit(BreakStmt* stmt) = 0;
    virtual Value visit(ReturnStmt* stmt) = 0;
    virtual Value visit(ExpStmt* stmt) = 0;
    virtual Value visit(Fun* fun) = 0;
    virtual void visit(Program* program) = 0;
};

#endif
