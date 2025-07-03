#ifndef RUSTY_GENCODE_H
#define RUSTY_GENCODE_H

#include "Visitor.h"

class CodeGen : public Visitor {
private:
    std::ostream& out;
public:
    CodeGen(SymbolTable* table, std::ostream& out): Visitor(table), out(out){}
    explicit CodeGen(std::ostream& out): Visitor(nullptr), out(out){}
    ~CodeGen() override;
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

#endif //RUSTY_GENCODE_H
