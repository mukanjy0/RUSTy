#include "Exp.h"
#include "Token.h"
#include <iostream>

Var::Type Var::stringToType(std::string type) {
    if (type == "bool") return BOOL;
    else if (type == "char") return CHAR;
    else if (type == "i32") return I32;
    else if (type == "str") return STR;
    else if (type == "()") return UNIT;
    throw std::runtime_error("invalid type: " + type);
}
std::ostream& operator<<(std::ostream& out, const Var& var) {
    if (var.size > 0) out << "[ ";
    int i {};
    switch(var.type) {
        case Var::BOOL: 
            for (auto el : var.numericValues) {
                out << std::boolalpha << (el > 0);
                if (++i < var.size) out << ", ";
            }
            break;
        case Var::CHAR:
            for (auto el : var.stringValues) {
                out << '\'' << el[0] << '\'';
                if (++i < var.size) out << ", ";
            }
            break;
        case Var::I32:
            for (auto el : var.numericValues) {
                out << el;
                if (++i < var.size) out << ", ";
            }
            break;
        case Var::STR:
            for (auto el : var.stringValues) {
                out << '"' << el << '"';
                if (++i < var.size) out << ", ";
            }
            break;
        case Var::UNIT: out << "()"; break;
        default: throw std::runtime_error("expected well-defined type for printing");
    }
    if (var.size > 0) out << "] ";
    return out;
}

Stmt::~Stmt() {}
std::ostream& operator<<(std::ostream& out, Stmt* stmt) {
    stmt->print(out);
    return out;
}

Block::~Block() {
    for (auto stmt : stmts) {
        delete stmt;
    }
    stmts.clear();
}
std::ostream& operator<<(std::ostream& out, Block* block) {
    out << "{" << "\n";
    for (auto stmt : block->stmts) {
        out << stmt << '\n';
    }
    out << "}" << "\n";
    return out;
}

Exp::~Exp() {}
std::ostream& operator<<(std::ostream& out, Exp* exp) {
    exp->print(out);
    return out;
}

BinaryExp::~BinaryExp() {
    delete lhs;
    delete rhs;
}
void BinaryExp::print(std::ostream& out) {
    out << lhs;
    switch (op) {
        case LAND: out << " && "; break;
        case LOR: out << " || "; break;
        case GT: out << " > "; break;
        case LT: out << " < "; break;
        case GE: out << " >= "; break;
        case LE: out << " <= "; break;
        case EQ: out << " == "; break;
        case NEQ: out << " != "; break;
        case PLUS: out << " + "; break;
        case MINUS: out << " - "; break;
        case TIMES: out << " * "; break;
        case DIV: out << " / "; break;
    }
    out << rhs;
}

UnaryExp::~UnaryExp() {
    delete exp;
}
void UnaryExp::print(std::ostream& out) {
    switch (op) {
        case LNOT: out << "! "; break;
    }
    out << exp;
}

Literal::~Literal() {}
void Literal::print(std::ostream& out) {
    out << value;
}

Variable::~Variable() {}
void Variable::print(std::ostream& out) {
    out << name;
}

void FunCall::print(std::ostream& out) {
    out << id << "(";
    size_t i {};
    for (auto exp : args) {
        out << exp;
        if (++i < args.size()) out << ", ";
    }
    out << ")";
}
FunCall::~FunCall() {
    for (auto exp : args) {
        delete exp;
    }
    args.clear();
}

IfBranch::~IfBranch() {
    delete cond;
    delete block;
}
std::ostream& operator<<(std::ostream& out, IfBranch ifBranch) {
    if (ifBranch.cond) out << ifBranch.cond << "\n";
    out << ifBranch.block;
    return out;
}
std::ostream& operator<<(std::ostream& out, IfBranch* ifBranch) {
    if (ifBranch->cond) out << ifBranch->cond << "\n";
    out << ifBranch->block;
    return out;
}

IfExp::~IfExp() {
    elseIfBranches.clear();
    delete elseBranch;
}
void IfExp::setIfBranch(const IfBranch& branch) { ifBranch = std::move(branch); }
void IfExp::setElseBranch(IfBranch* branch) {elseBranch = branch; }
void IfExp::print(std::ostream& out) {
    out << "if " << ifBranch;
    for (auto ifBranch : elseIfBranches) {
        out << "else if " << ifBranch;
    }
    if (elseBranch) out << "else" << elseBranch;
}

LoopExp::~LoopExp() {
    delete block;
}
void LoopExp::print(std::ostream& out) {
    out << "loop";
    out << block;
}

SubscriptExp::~SubscriptExp() {
    delete exp;
}
void SubscriptExp::print(std::ostream& out) {
    out << id << '[' << exp << ']';
}

SliceExp::~SliceExp() {
    delete start;
    delete end;
}
void SliceExp::print(std::ostream& out) {
    out << id << '[';
    if (start) out << start;
    out << "..";
    if (inclusive) out << '=';
    if (end) out << end;
    out << ']';
}

ReferenceExp::~ReferenceExp() {
    delete exp;
}
void ReferenceExp::print(std::ostream& out) {
    out << std::string(count, '&');
    out << exp;
}

ArrayExp::~ArrayExp() {
    for (auto el : elements) {
        delete el;
    }
    elements.clear();
}
void ArrayExp::print(std::ostream& out) {
    out << '[';
    size_t i {};
    for (auto el : elements) {
        out << el;
        if (++i < elements.size()) out << ", ";
    }
    out << ']';
}

UniformArrayExp::~UniformArrayExp() {
    delete value;
    delete size;
}
void UniformArrayExp::print(std::ostream& out) {
    out << '[' << value << "; " << size << ']';
}
