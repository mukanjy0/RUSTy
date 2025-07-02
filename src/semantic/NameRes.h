//
// Created by jose on 01/07/25.
//

#ifndef NAMERES_H
#define NAMERES_H

#include "Visitor.h"

class NameRes final : public Visitor {
public:
    explicit NameRes(SymbolTable* table) : Visitor(table) {}
    ~NameRes() override;
    Var visit(Block* block) override;
    Var visit(BinaryExp* exp) override;
    Var visit(UnaryExp* exp) override;
    Var visit(Literal* exp) override;
    Var visit(Variable* exp) override;
    Var visit(FunCall* exp) override;
    Var visit(IfExp* exp) override;
    Var visit(LoopExp* exp) override;
    Var visit(SubscriptExp* exp) override;
    Var visit(SliceExp* exp) override;
    Var visit(ReferenceExp* exp) override;
    void visit(DecStmt* stmt) override;
    void visit(AssignStmt* stmt) override;
    void visit(CompoundAssignStmt* stmt) override;
    void visit(ForStmt* stmt) override;
    void visit(WhileStmt* stmt) override;
    void visit(PrintStmt* stmt) override;
    void visit(BreakStmt* stmt) override;
    void visit(ReturnStmt* stmt) override;
    void visit(ExpStmt* stmt) override;
    void visit(Fun* fun) override;
    void visit(Program* program) override;
};



#endif //NAMERES_H
