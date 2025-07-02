//
// Created by jose on 01/07/25.
//

#include "NameRes.h"

// Destructor
NameRes::~NameRes() = default;

// Visit methods for expressions
Var NameRes::visit(Block* block) {
    for(auto stmt : block->stmts) {
        stmt->accept(this);
    }
    return {};
}

Var NameRes::visit(BinaryExp* exp) {
    exp->lhs->accept(this);
    exp->rhs->accept(this);

    switch (exp->op) {
        case BinaryExp::LAND:
            break;
        case BinaryExp::LOR:
            break;
        case BinaryExp::GT:
            break;
        case BinaryExp::LT:

            break;
        case BinaryExp::GE:
            break;
        case BinaryExp::LE:
            break;
        case BinaryExp::EQ:

            break;
        case BinaryExp::NEQ:

            break;
        case BinaryExp::PLUS:

            break;
        case BinaryExp::MINUS:

            break;
        case BinaryExp::TIMES:

            break;
        case BinaryExp::DIV:

            break;
        default:
            throw std::runtime_error("Invalid binary operation");
    }
    return {};

}

Var NameRes::visit(UnaryExp* exp) {
    exp->exp->accept(this);

    switch (exp->op) {
        case UnaryExp::LNOT:
            break;
        default:
            throw std::runtime_error("Invalid unary operation");
    }
    return {};
}

Var NameRes::visit(Number* exp) {
    return {Var::I32, 0};
}

Var NameRes::visit(Variable* exp) {
    // Implementation here
    return {};
}

Var NameRes::visit(FunCall* exp) {
    // Implementation here
    return {};
}

Var NameRes::visit(IfExp* exp) {
    // Implementation here
    return {};
}

Var NameRes::visit(LoopExp* exp) {
    // Implementation here
    return {};
}

// Visit methods for statements
void NameRes::visit(DecStmt* stmt) {
    // Implementation here
}

void NameRes::visit(AssignStmt* stmt) {
    // Implementation here
}

void NameRes::visit(ForStmt* stmt) {
    // Implementation here
}

void NameRes::visit(WhileStmt* stmt) {
    // Implementation here
}

void NameRes::visit(PrintStmt* stmt) {
    // Implementation here
}

void NameRes::visit(BreakStmt* stmt) {
    // Implementation here
}

void NameRes::visit(ReturnStmt* stmt) {
    // Implementation here
}

void NameRes::visit(ExpStmt* stmt) {
    // Implementation here
}

// Visit methods for functions and programs
void NameRes::visit(Fun* fun) {
    // Implementation here
}

void NameRes::visit(Program* program) {
    for (const auto& [id, fun] : program->funs) {

    }
}