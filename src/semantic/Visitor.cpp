#include "Visitor.h"

// ACCEPT

Value Block::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value UnaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value Literal::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value Valueiable::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value FunCall::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value IfExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value LoopExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value SubscriptExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value SliceExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value ReferenceExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value ArrayExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value UniformArrayExp::accept(Visitor* visitor) {
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

