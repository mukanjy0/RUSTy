#include "Stmt.h"

DecStmt::~DecStmt() {
    delete rhs;
}
void DecStmt::print(std::ostream& out) {
    out << "let ";
    if (var.mut) out << "mut ";
    out << id;
    if (var.type != Value::UNDEFINED) out << ": ";
    if (var.ref) out << '&';
    switch(var.type) {
        case Value::BOOL: out << "bool"; break;
        case Value::CHAR: out << "char"; break;
        case Value::I32: out << "i32"; break;
        case Value::I64: out << "i64"; break;
        case Value::STR: out << "str"; break;
        case Value::UNIT: out << "()"; break;
        default: break;
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
    if (ref) out << '&';
    out << lhs << " = " << rhs << ";";
}

CompoundAssignStmt::~CompoundAssignStmt() {
    delete rhs;
}
void CompoundAssignStmt::print(std::ostream& out) {
    out << lhs;
    switch(op) {
        case BinaryExp::PLUS: out << " +"; break;
        case BinaryExp::MINUS: out << " -"; break;
        case BinaryExp::TIMES: out << " *"; break;
        case BinaryExp::DIV: out << " /"; break;
        default: throw std::runtime_error("invalid compound assign operation");
    }
    out << "= " << rhs << ";";
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
    out << exp;
    if (!returnValue) out << ";";
}
