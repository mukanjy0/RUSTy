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
Visitor::~Visitor() = default;
