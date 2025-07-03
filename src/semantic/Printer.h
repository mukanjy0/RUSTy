//
// Created by jose on 01/07/25.
//

#ifndef PRINTER_H
#define PRINTER_H

#include "Visitor.h"

class Printer : public Visitor {
public:
    Printer() = default;
    ~Printer() override;
    Val visit(Block* block) override;
    Val visit(BinaryExp* exp) override;
    Val visit(UnaryExp* exp) override;
    Val visit(Literal* exp) override;
    Val visit(Variable* exp) override;
    Val visit(FunCall* exp) override;
    Val visit(IfExp* exp) override;
    Val visit(LoopExp* exp) override;
    Val visit(SubscriptExp* exp) override;
    Val visit(SliceExp* exp) override;
    Val visit(ReferenceExp* exp) override;
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



#endif //PRINTER_H
