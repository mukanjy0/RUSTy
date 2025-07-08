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
    Operand(L lvl) : lvl(lvl) {}
    virtual ~Operand() = 0;
    virtual void print(std::ostream &out) = 0;
    friend ostream& operator<<(ostream& out, Operand* op);
};

class Reg : public Operand {
    friend class CodeGen;

    string reg {"a"};
    Reg(L lvl = Q) : Operand(lvl) {}
    Reg(string reg, L lvl = Q) : Operand(lvl), reg(reg) {}
public:
    ~Reg();
    void print(std::ostream &out) override;
};

class Const : public Operand {
    friend class CodeGen;

    Value value {};
    Const(Value value, L lvl = Q) : Operand(lvl), value(value) {
        switch(value.type) {
            case Value::CHAR:
                value.numericValues.push_back(
                    value.stringValues.front()[0]
                );
            case Value::I8:
            case Value::BOOL:
                lvl = B;
                break;
            case Value::I16:
                lvl = W;
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
    Mem(Reg* reg, int offset, L lvl = Q) 
    : Operand(lvl), reg(reg), offset(offset) {
        if (reg->lvl != Q) throw runtime_error("nonono");
    }
    Mem(Reg* reg, string label, L lvl = Q) 
    : Operand(lvl), reg(reg), label(label) {
        if (reg->lvl != Q) throw runtime_error("nonono");
    }
    Mem(Reg* reg, int offset, string label, L lvl = Q) 
    : Operand(lvl), reg(reg), offset(offset), label(label) {
        if (reg->lvl != Q) throw runtime_error("nonono");
    }
public:
    ~Mem();
    void print(std::ostream &out) override;
};

class CodeGen : public Visitor {
private:
    ostream& out;

    char nextL(L lvl);
    L valueToL(Value value);
    L typeToL(Value::Type type);

    void mov();
    void movz();
    void movs();
    // arithmetic
    void add();
    void addSP(int off, bool early=false);
    void inc();
    void sub();
    void subSP(int off);
    void dec();
    void mul();
    void div();
    void land();
    void lor();
    void lnot();
    void bnot();
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
    void leave(bool early=false);
    void ret();

    string LCLabel();
    void LBLabel();
    void LELabel();
    string LIBLabel();
    void LIBLabel2();
    void LIELabel();
    string nextIf();
    void LFBLabel();
    void LFELabel();
    string end(string label);
    int getOffset(string label);

    int lb {};
    int lc {};
    int lf {};
    int lib {};
    int lie {};
    stack<int> lbs;
    stack<int> lis;
    stack<int> bp {};
    stack<string> labels {};
    string curFun {};
    int offset {};
    bool init {};
    Operand* l;
    Operand* r;
    stack<Block*> cur;
    map<Block*, int> allocated;
    map<Block*, int> toAllocate;

    // labels for boolean printing
    std::string boolTrueLabel;
    std::string boolFalseLabel;

public:
    CodeGen(SymbolTable* table, std::ostream& out)
        : Visitor(table), out(out), boolTrueLabel(), boolFalseLabel() {}
    explicit CodeGen(std::ostream& out)
        : Visitor(nullptr), out(out), boolTrueLabel(), boolFalseLabel() {}
    ~CodeGen() override;
    static int typeLen(L lvl);
    static int typeLen(Value value);
    static int typeLen(Value::Type type);
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
    Value accept(Block* block);
    Value accept(Exp* exp);
    Value accept(Stmt* stmt);
    Value accept(Fun* fun);
};

#endif //RUSTY_GENCODE_H
