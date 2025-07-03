#include "Visitor.h"

// ACCEPT

Var Block::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var UnaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var Literal::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var Variable::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var FunCall::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var IfExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var LoopExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var SubscriptExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var SliceExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var ReferenceExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var ArrayExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Var UniformArrayExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void DecStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void AssignStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void CompoundAssignStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void ForStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void WhileStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void PrintStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void BreakStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void ReturnStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void ExpStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void Fun::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// VISITOR
Visitor::~Visitor() {}

// PRINT VISITOR

Printer::~Printer() {}
Var Printer::visit(Block* block) {
    std::cout << block;
    return Var();
}

Var Printer::visit(BinaryExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(UnaryExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(Literal* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(Variable* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(FunCall* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(IfExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(LoopExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(SubscriptExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(SliceExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(ReferenceExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(ArrayExp* exp) {
    std::cout << exp;
    return Var();
}

Var Printer::visit(UniformArrayExp* exp) {
    std::cout << exp;
    return Var();
}

void Printer::visit(DecStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(AssignStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(CompoundAssignStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(ForStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(WhileStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(PrintStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(BreakStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(ReturnStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(ExpStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(Fun* fun) {
    std::cout << fun;
}

void Printer::visit(Program* program) {
    std::cout << program;
}

