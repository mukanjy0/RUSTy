#include "Stmt.h"
#include <iostream>

DecStmt::~DecStmt() {
    delete rhs;
}
void DecStmt::print(std::ostream& out) {
    out << "let ";
    if (var.mut) out << "mut ";
    out << id << " :";
    switch(var.type) {
        case Var::BOOL: out << "bool"; break;
        case Var::CHAR: out << "char"; break;
        case Var::I32: out << "i32"; break;
        case Var::STR: out << "String"; break;
        case Var::VOID: out << "void"; break;
        default: out << "undefined"; break;
    }
    if (rhs) {
        out << " = " << rhs;
    }
    out << ";";
}

AssignStmt::~AssignStmt() {
    delete rhs;
}
void AssignStmt::print(std::ostream& out) {
    out << lhs << " = " << rhs << ";";
}

ForStmt::~ForStmt() {
    delete start;
    delete end;
    delete block;
}
void ForStmt::print(std::ostream& out) {
    out << "for " << id << " in " << start << "..";
    if (inclusive) out << '=';
    out << end << "\n";
    out << block;
}

WhileStmt::~WhileStmt() {
    delete cond;
    delete block;
}
void WhileStmt::print(std::ostream& out) {
    out << "while " << cond << '\n';
    out << block;
}

PrintStmt::~PrintStmt() {
    for (auto exp : args) {
        delete exp;
    }
    args.clear();
}
void PrintStmt::print(std::ostream& out) {
    out << "println!(\"" << strLiteral << '"';
    if (!args.empty()) {
        out << ", ";
        size_t i {};
        for (auto exp : args) {
            out << exp;
            if (++i < args.size()) out << ", ";
        }
    }
    out << ");";
}

BreakStmt::~BreakStmt() {
    delete exp;
}
void BreakStmt::print(std::ostream& out) {
    out << "break";
    if (exp) out << ' ' << exp;
    out << ";";
}

ReturnStmt::~ReturnStmt() {
    delete exp;
}
void ReturnStmt::print(std::ostream& out) {
    out << "return";
    if (exp) out << ' ' << exp;
    out << ";";
}

ExpStmt::~ExpStmt() {
    delete exp;
}
void ExpStmt::print(std::ostream& out) {
    out << exp << ";";
}
