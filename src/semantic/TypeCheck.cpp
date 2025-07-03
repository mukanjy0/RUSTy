//
// Created by jose on 01/07/25.
//

#include "TypeCheck.h"

// Destructor
TypeCheck::~TypeCheck() = default;

// Visit methods for expressions
Value TypeCheck::visit(Block* block) {
    for(auto stmt : block->stmts) {
        stmt->accept(this);
    }
    return {};
}

Value TypeCheck::visit(BinaryExp* exp) {
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

Value TypeCheck::visit(UnaryExp* exp) {
    exp->exp->accept(this);

    switch (exp->op) {
        case UnaryExp::LNOT:
            break;
        default:
            throw std::runtime_error("Invalid unary operation");
    }
    return {};
}

Value TypeCheck::visit(Literal* exp) {
    return {Value::I32, 0};
}

Value TypeCheck::visit(Variable* exp) {
    // Implementation here
    return {};
}

Value TypeCheck::visit(FunCall* exp) {
    // Implementation here
    return {};
}

Value TypeCheck::visit(IfExp* exp) {
    // Implementation here
    return {};
}

Value TypeCheck::visit(LoopExp* exp) {
    // Implementation here
    return {};
}

Value TypeCheck::visit(SubscriptExp* exp) {
    return {};
}

Value TypeCheck::visit(SliceExp* exp) {
    return {};
}
Value TypeCheck::visit(ReferenceExp* exp) {
    return {};
}

// Visit methods for statements
void TypeCheck::visit(DecStmt* stmt) {
    // Implementation here
}

void TypeCheck::visit(AssignStmt* stmt) {
    // Implementation here
}

void TypeCheck::visit(ForStmt* stmt) {
    // Implementation here
}

void TypeCheck::visit(WhileStmt* stmt) {
    // Implementation here
}

void TypeCheck::visit(PrintStmt* stmt) {
    // Implementation here
}

void TypeCheck::visit(BreakStmt* stmt) {
    // Implementation here
}

void TypeCheck::visit(ReturnStmt* stmt) {
    // Implementation here
}

void TypeCheck::visit(ExpStmt* stmt) {
    // Implementation here
}

// Visit methods for functions and programs
void TypeCheck::visit(Fun* fun) {
    // Implementation here
}

void TypeCheck::visit(Program* program) {
    for (const auto& [id, fun] : program->funs) {

    }
}

void TypeCheck::visit(CompoundAssignStmt* stmt) {

}