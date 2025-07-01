#ifndef RUSTY_GENCODE_H
#define RUSTY_GENCODE_H

#include "Visitor.h"

class CodeGen : public Visitor {
private:
    std::ostream& out;
public:
    CodeGen(std::ostream& out):out(out){};
    ~CodeGen();
    Var visit(Block* block);
    Var visit(BinaryExp* exp);
    Var visit(UnaryExp* exp);
    Var visit(Number* exp);
    Var visit(Variable* exp);
    Var visit(FunCall* exp);
    Var visit(IfExp* exp);
    Var visit(LoopExp* exp);
    void visit(DecStmt* stmt);
    void visit(AssignStmt* stmt);
    void visit(ForStmt* stmt);
    void visit(WhileStmt* stmt);
    void visit(PrintStmt* stmt);
    void visit(BreakStmt* stmt);
    void visit(ReturnStmt* stmt);
    void visit(ExpStmt* stmt);
    void visit(Fun* fun);
    void visit(Program* program);
};

#endif //RUSTY_GENCODE_H
