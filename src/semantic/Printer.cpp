#include "Printer.h"

// PRINT VISITOR

Printer::~Printer() = default;
Value Printer::visit(Block* block) {
    std::cout << block;
    return {};
}

Value Printer::visit(BinaryExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(UnaryExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(Literal* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(Variable* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(FunCall* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(IfExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(LoopExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(SubscriptExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(SliceExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(ReferenceExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(ArrayExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(UniformArrayExp* exp) {
    std::cout << exp;
    return {};
}

Value Printer::visit(DecStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(AssignStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(CompoundAssignStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(ForStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(WhileStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(PrintStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(BreakStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(ReturnStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(ExpStmt* stmt) {
    std::cout << stmt;
    return {};
}

Value Printer::visit(Fun* fun) {
    std::cout << fun;
    return {};
}

void Printer::visit(Program* program) {
    std::cout << program;
}

