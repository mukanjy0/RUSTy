#include "Printer.h"

Printer::~Printer() {}
Value Printer::visit(Block* block) {
    std::cout << block;
    return Value();
}

Value Printer::visit(BinaryExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(UnaryExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(Literal* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(Variable* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(FunCall* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(IfExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(LoopExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(SubscriptExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(SliceExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(ReferenceExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(ArrayExp* exp) {
    std::cout << exp;
    return Value();
}

Value Printer::visit(UniformArrayExp* exp) {
    std::cout << exp;
    return Value();
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

