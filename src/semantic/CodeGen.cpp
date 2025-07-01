#include "CodeGen.h"

// Destructor
CodeGen::~CodeGen() = default;

// Visit methods for expressions
Var CodeGen::visit(Block* block) {
    for(auto stmt : block->stmts) {
        stmt->accept(this);
    }
    return {};
}

Var CodeGen::visit(BinaryExp* exp) {
    exp->lhs->accept(this);
    out << " pushq %rax\n";
    exp->rhs->accept(this);
    out << " movq %rax, %rcx\n";
    out << " popq %rax\n";

    switch (exp->op) {
        case BinaryExp::LAND:
            out << " andq %rcx, %rax\n";
            break;
        case BinaryExp::LOR:
            out << " orq %rcx, %rax\n";
            break;
        case BinaryExp::GT:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setg %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::LT:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setl %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::GE:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setge %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::LE:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setle %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::EQ:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " sete %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::NEQ:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setne %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::PLUS:
            out << " addq %rcx, %rax\n";
            break;
        case BinaryExp::MINUS:
            out << " subq %rcx, %rax\n";
            break;
        case BinaryExp::TIMES:
            out << " imulq %rcx, %rax\n";
            break;
        case BinaryExp::DIV:
            out  << " cqto\n"
                 << " idivq %rcx\n";
            break;
        default:
            throw std::runtime_error("Invalid binary operation");
    }
    return {};

}

Var CodeGen::visit(UnaryExp* exp) {
    exp->exp->accept(this);
    out << " movq %rax, %rcx\n";

    switch (exp->op) {
        case UnaryExp::LNOT:
            out << " notq %rcx\n";
            break;
        default:
            throw std::runtime_error("Invalid unary operation");
    }

    out << " movq %rcx, %rax\n";
    return {};
}

Var CodeGen::visit(Number* exp) {
    out << " movq $" << exp->value << ", %rax\n";
    return {};
}

Var CodeGen::visit(Variable* exp) {
    // Implementation here
    return {};
}

Var CodeGen::visit(FunCall* exp) {
    // Implementation here
    return {};
}

Var CodeGen::visit(IfExp* exp) {
    // Implementation here
    return {};
}

Var CodeGen::visit(LoopExp* exp) {
    // Implementation here
    return {};
}

// Visit methods for statements
void CodeGen::visit(DecStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(AssignStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(ForStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(WhileStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(PrintStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(BreakStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(ReturnStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(ExpStmt* stmt) {
    // Implementation here
}

// Visit methods for functions and programs
void CodeGen::visit(Fun* fun) {
    // Implementation here
}

void CodeGen::visit(Program* program) {
    // Implementation here
}