#ifndef RUSTY_GENCODE_H
#define RUSTY_GENCODE_H

#include "Visitor.h"

using namespace std;

class CodeGen : public Visitor {
private:
    enum L {B, W, D, Q};
    enum C {NONE, EQ, NE, GT, LT};
    ostream& out;

    struct Operand {
        string reg {"a"}; // for regs
        L lvl {L::Q};
        int value {}; // for constant
        int offset {}; // for memory
        bool mem {};
        bool constant {};
        string label {};

        operator string() const;
    };

    char suffix(L lvl);
    char suffix(Operand o);
    char suffix(Value value);
    char suffix(Value::Type type);
    L valueToL(Value value);
    L typeToL(Value::Type type);

    void mov(Operand l, Operand r);
    void movz(Operand l, Operand r);
    // arithmetic
    void add(Operand l, Operand r);
    void inc(Operand o);
    void sub(Operand l, Operand r);
    void dec(Operand o);
    void mul(Operand l, Operand r);
    void div(Operand l, Operand r);
    // stack
    void push(Operand o);
    void pop(Operand o);
    // memory
    void lea(Operand l, Operand r);
    // conditional
    void cmp(Operand l, Operand r);
    void jmp(Operand r, C=NONE);
    void set(Operand r, C=NONE);
    void cmov(Operand l, Operand r, C=NONE);

    friend ostream& operator<<(ostream& out, C cond) {
        switch (cond) {
            case EQ: out << "e"; break;
            case NE: out << "ne"; break;
            case GT: out << "g"; break;
            case LT: out << "l"; break;
            default: break;
        }
        return out;
    }
    friend ostream& operator<<(ostream& out, const Operand& op) {
        out << string(op);
        return out;
    }

    int count {};

public:
    explicit CodeGen(std::ostream& out): Visitor(), out(out){}
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
    Value visit(DecStmt* stmt) override;
    Value visit(AssignStmt* stmt) override;
    Value visit(CompoundAssignStmt* stmt) override;
    Value visit(ForStmt* stmt) override;
    Value visit(WhileStmt* stmt) override;
    Value visit(PrintStmt* stmt) override;
    Value visit(BreakStmt* stmt) override;
    Value visit(ReturnStmt* stmt) override;
    Value visit(ExpStmt* stmt) override;
    Value visit(Fun* fun) override;
    Value visit(Program* program) override;
};

#endif //RUSTY_GENCODE_H
