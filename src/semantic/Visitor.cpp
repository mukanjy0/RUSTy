#include "Visitor.h"

// ACCEPT

Val Block::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val UnaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val Literal::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val Variable::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val FunCall::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val IfExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val LoopExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val SubscriptExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val SliceExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
Val ReferenceExp::accept(Visitor* visitor) {
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
