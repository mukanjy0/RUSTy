#ifndef RUSTY_GENCODE_H
#define RUSTY_GENCODE_H

#include "Visitor.h"
#include <map>
#include <stack>

using namespace std;

enum L {B, W, D, Q};
enum C {NONE, EQ, NE, GT, LT, GE, LE};

class Operand {
public:
    L lvl {L::Q};
    virtual ~Operand() = 0;
    virtual void print(std::ostream &out) = 0;
    friend ostream& operator<<(ostream& out, Operand* op);
};

class Reg : public Operand {
    friend class CodeGen;

    string reg {"a"};
    Reg() {}
    Reg(string reg) : reg(reg) {}
public:
    ~Reg();
    void print(std::ostream &out) override;
};

class Const : public Operand {
    friend class CodeGen;

    Value value {};
    Const(Value value) : value(value) {
        switch(value.type) {
            case Value::CHAR:
                value.numericValues.push_back(
                    value.stringValues.front()[0]
                );
            case Value::BOOL:
                lvl = B;
                break;
            case Value::I32:
                lvl = D;
                break;
            default: lvl = Q;
        }
    }
public:
    ~Const();
    void print(std::ostream &out) override;
};

class Mem : public Operand {
    friend class CodeGen;

    Reg* reg;
    int offset {};
    string label {};
    Mem(Reg* reg, int offset) 
    : reg(reg), offset(offset) {}
    Mem(Reg* reg, string label) 
    : reg(reg), label(label) {}
    Mem(Reg* reg, int offset, string label) 
    : reg(reg), offset(offset), label(label) {}
public:
    ~Mem();
    void print(std::ostream &out) override;
};

class CodeGen : public Visitor {
private:
    ostream& out;

    L valueToL(Value value);
    L typeToL(Value::Type type);

    void mov();
    void movz();
    // arithmetic
    void add();
    void inc();
    void sub();
    void dec();
    void mul();
    void div();
    // stack
    void push();
    void pop();
    // memory
    void lea();
    // conditional
    void cmp();
    void jmp(C=NONE);
    void jmp(string label, C=NONE);
    void set(C=NONE);
    void cmov(C=NONE);
    void call(string label);
    void enter();
    void leave();
    void ret();

    string LCLabel();
    void LBLabel();
    void LELabel();
    void LFBLabel();
    void LFELabel();
    string end(string label);

    int lb {};
    int lc {};
    int lf {};
    stack<int> lbs;
    stack<int> bp {};
    stack<string> labels {};
    int offset {};
    int prevOff {};
    bool init {};
    Operand* l;
    Operand* r;
    stack<Block*> cur;
    map<Block*, int> allocated;
    map<Block*, int> toAllocate;

public:
    CodeGen(SymbolTable* table, std::ostream& out): Visitor(table), out(out){}
    explicit CodeGen(std::ostream& out): Visitor(nullptr), out(out){}
    ~CodeGen() override;
    static int typeLen(Value::Type type);
    static int typeLen(L lvl);
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
    void visit(Program* program) override;
};

#endif //RUSTY_GENCODE_H
