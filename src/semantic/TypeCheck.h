//
// Created by jose on 01/07/25.
//

#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "Visitor.h"

class TypeCheck final : public Visitor {
public:
    explicit TypeCheck(SymbolTable* table = nullptr) : Visitor(table) {}
    ~TypeCheck() override;
    Value visit(Block* block) override;
    Value visit(BinaryExp* exp) override;
    Value visit(UnaryExp* exp) override;
    Value visit(Literal* exp) override;
    Value visit(Variable* exp) override;
    Value visit(FunCall* exp) override;
    Value visit(IfExp* exp) override;
    Value visit(LoopExp* exp) override;
    Value visit(SubscriptExp* exp) override;
    Value visit(SliceExp* exp) override;
    Value visit(ReferenceExp* exp) override;
    Value visit(ArrayExp* exp) override;
    Value visit(UniformArrayExp* exp) override;
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

#endif //TYPECHECK_H
