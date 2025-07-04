#include "CodeGen.h"

// Destructor
CodeGen::~CodeGen() = default;

// Visit methods for expressions
Value CodeGen::visit(Block* block) {
    for(auto stmt : block->stmts) {
        stmt->accept(this);
    }
    return {};
}

Value CodeGen::visit(BinaryExp* exp) {
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

Value CodeGen::visit(UnaryExp* exp) {
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

Value CodeGen::visit(Literal* exp) {
    out << " movq $" << exp->value << ", %rax\n";
    return {};
}

Value CodeGen::visit(Variable* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(FunCall* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(IfExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(LoopExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(SubscriptExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(SliceExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(ReferenceExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(ArrayExp* exp) {
    std::cout << exp;
    return {};
}

Value CodeGen::visit(UniformArrayExp* exp) {
    std::cout << exp;
    return {};
}

// Visit methods for statements
Value CodeGen::visit(DecStmt* stmt) {
    return {};
}

Value CodeGen::visit(AssignStmt* stmt) {
    return {};
}

Value CodeGen::visit(CompoundAssignStmt* stmt) {
    return {};
}

Value CodeGen::visit(ForStmt* stmt) {
    return {};
}

Value CodeGen::visit(WhileStmt* stmt) {
    return {};
}

Value CodeGen::visit(PrintStmt* stmt) {
    return {};
}

Value CodeGen::visit(BreakStmt* stmt) {
    return {};
}

Value CodeGen::visit(ReturnStmt* stmt) {
    return {};
}

Value CodeGen::visit(ExpStmt* stmt) {
    return {};
}

// Visit methods for functions and programs
Value CodeGen::visit(Fun* fun) {
    return {};
}

void CodeGen::visit(Program* program) {
}