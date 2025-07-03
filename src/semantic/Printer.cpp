//
// Created by jose on 01/07/25.
//

#include "Printer.h"

// PRINT VISITOR

Printer::~Printer() = default;
Val Printer::visit(Block* block) {
    std::cout << block;
    return {};
}

Val Printer::visit(BinaryExp* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(UnaryExp* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(Literal* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(Variable* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(FunCall* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(IfExp* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(LoopExp* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(SubscriptExp* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(SliceExp* exp) {
    std::cout << exp;
    return {};
}

Val Printer::visit(ReferenceExp* exp) {
    std::cout << exp;
    return {};
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

