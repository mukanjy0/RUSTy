#include <iostream>
#include "Exp.h"

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

Var Exp::accept(Visitor* visitor) {
    return Var();
}
std::ostream& operator<<(std::ostream& out, Exp* exp) {
    exp->print(out);
    return out;
}

IfBranch::~IfBranch() {
    delete cond;
    delete block;
}
std::ostream& operator<<(std::ostream& out, IfBranch* ifBranch) {
    out << ifBranch->cond << "\n";
    out << ifBranch->block;
    return out;
}

IfExp::~IfExp() {
    delete ifBranch;
    for (auto ifBranch : elseIfBranches) {
        delete ifBranch;
    }
    elseIfBranches.clear();
    delete elseBranch;
}
void IfExp::print(std::ostream& out) {
    out << "if " << ifBranch;
    for (auto ifBranch : elseIfBranches) {
        out << "else if " << ifBranch;
    }
    if (elseBranch) out << "else " << elseBranch;
}

LoopExp::~LoopExp() {
    delete block;
}
void LoopExp::print(std::ostream& out) {
    out << "loop";
    out << block;
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
        case LNOT: out << " ! "; break;
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

Number::~Number() {}
void Number::print(std::ostream& out) {
    out << value;
}
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
