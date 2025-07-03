#ifndef PRINTER_H
#define PRINTER_H

#include "Visitor.h"

class Printer : public Visitor {
public:
    Printer() = default;
    ~Printer();
    Value visit(Block* block);
    Value visit(BinaryExp* exp);
    Value visit(UnaryExp* exp);
    Value visit(Literal* exp);
    Value visit(Valueiable* exp);
    Value visit(FunCall* exp);
    Value visit(IfExp* exp);
    Value visit(LoopExp* exp);
    Value visit(SubscriptExp* exp);
    Value visit(SliceExp* exp);
    Value visit(ReferenceExp* exp);
    Value visit(ArrayExp* exp);
    Value visit(UniformArrayExp* exp);
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

#endif
