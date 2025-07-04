#ifndef RUSTY_GENCODE_H
#define RUSTY_GENCODE_H

#include "Visitor.h"

using namespace std;

class CodeGen : public Visitor {
private:
    enum L {B, W, D, Q};
    ostream& out;

    struct Operand {
        L lvl {L::Q};
        bool mem {};
        bool constant {};
        int offset {}; // for memory
        int value {}; // for constant
        string reg {"a"}; // for regs

        operator string() const;
    };

    char suffix(L lvl);
    char suffix(Value value);
    char suffix(Value::Type type);
    L valueToL(Value value);
    L typeToL(Value::Type type);

    void mov(Value value, Operand l, Operand r);
    // arithmetic
    void add(Value value, Operand l, Operand r);
    void inc(Value value, Operand o);
    void sub(Value value, Operand l, Operand r);
    void dec(Value value, Operand o);
    void mul(Value value, Operand l, Operand r);
    void div(Value value, Operand l, Operand r);
    // stack
    void push(Value value, Operand o);
    void pop(Value value, Operand o);
    // memory
    void lea(Value value, Operand l, Operand r);

    friend ostream& operator<<(ostream& out, const Operand& op) {
        out << string(op);
        return out;
    }

public:
    CodeGen(SymbolTable* table, std::ostream& out): Visitor(table), out(out){}
    explicit CodeGen(std::ostream& out): Visitor(nullptr), out(out){}
    ~CodeGen() override;
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

#endif //RUSTY_GENCODE_H
