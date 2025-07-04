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
Value Variable::accept(Visitor* visitor) {
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
Value DecStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value AssignStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value CompoundAssignStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value ForStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value WhileStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value PrintStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value BreakStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value ReturnStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value ExpStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Value Fun::accept(Visitor* visitor) {
    return visitor->visit(this);
}
void Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// VISITOR
Visitor::~Visitor() = default;
