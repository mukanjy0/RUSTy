#ifndef RUSTY_GENCODE_H
#define RUSTY_GENCODE_H

#include "Visitor.h"

class CodeGen : public Visitor {
private:
    std::ostream& out;
public:
    explicit CodeGen(std::ostream& out):out(out){};
    ~CodeGen() override;
    Var visit(Block* block) override;
    Var visit(BinaryExp* exp) override;
    Var visit(UnaryExp* exp) override;
    Var visit(Number* exp) override;
    Var visit(Variable* exp) override;
    Var visit(FunCall* exp) override;
    Var visit(IfExp* exp) override;
    Var visit(LoopExp* exp) override;
    void visit(DecStmt* stmt) override;
    void visit(AssignStmt* stmt) override;
    void visit(ForStmt* stmt) override;
    void visit(WhileStmt* stmt) override;
    void visit(PrintStmt* stmt) override;
    void visit(BreakStmt* stmt) override;
    void visit(ReturnStmt* stmt) override;
    void visit(ExpStmt* stmt) override;
    void visit(Fun* fun) override;
    void visit(Program* program) override;
};

#endif //RUSTY_GENCODE_H
