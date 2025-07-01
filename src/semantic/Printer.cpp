//
// Created by jose on 01/07/25.
//

#include "Printer.h"

// PRINT VISITOR

Printer::~Printer() = default;
Var Printer::visit(Block* block) {
    std::cout << block;
    return {};
}

Var Printer::visit(BinaryExp* exp) {
    std::cout << exp;
    return {};
}

Var Printer::visit(UnaryExp* exp) {
    std::cout << exp;
    return {};
}

Var Printer::visit(Number* exp) {
    std::cout << exp;
    return {};
}

Var Printer::visit(Variable* exp) {
    std::cout << exp;
    return {};
}

Var Printer::visit(FunCall* exp) {
    std::cout << exp;
    return {};
}

Var Printer::visit(IfExp* exp) {
    std::cout << exp;
    return {};
}

Var Printer::visit(LoopExp* exp) {
    std::cout << exp;
    return {};
}

void Printer::visit(DecStmt* stmt) {
    std::cout << stmt;
}

void Printer::visit(AssignStmt* stmt) {
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

