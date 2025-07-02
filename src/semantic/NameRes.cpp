#include "NameRes.h"
#include <iostream>
#include <ranges>

NameRes::~NameRes() = default;

Var NameRes::visit(Block* block) {
    table->pushScope();
    for (auto stmt : block->stmts) {
        stmt->accept(this);
    }
    table->popScope();
    return {};
}

Var NameRes::visit(BinaryExp* exp) {
    exp->lhs->accept(this);
    exp->rhs->accept(this);
    return {};
}

Var NameRes::visit(UnaryExp* exp) {
    exp->exp->accept(this);
    return {};
}

Var NameRes::visit(Number* exp) {
    (void)exp; // unused
    return {};
}

Var NameRes::visit(Variable* exp) {
    if (!table->lookup(exp->name)) {
        std::cerr << "Name resolution error: undefined variable '"
                  << exp->name << "'\n";
    }
    return {};
}

Var NameRes::visit(FunCall* exp) {
    for (auto arg : exp->args) {
        arg->accept(this);
    }
    return {};
}

Var NameRes::visit(IfExp* exp) {
    exp->ifBranch.cond->accept(this);
    exp->ifBranch.block->accept(this);
    for (auto& br : exp->elseIfBranches) {
        br.cond->accept(this);
        br.block->accept(this);
    }
    if (exp->elseBranch) {
        exp->elseBranch->block->accept(this);
    }
    return {};
}

Var NameRes::visit(LoopExp* exp) {
    exp->block->accept(this);
    return {};
}

void NameRes::visit(DecStmt* stmt) {
    if (!table->declare(stmt->id, stmt->var)) {
        std::cerr << "Name resolution error: redeclaration of '"
                  << stmt->id << "'\n";
    }
    if (stmt->rhs) {
        stmt->rhs->accept(this);
    }
}

void NameRes::visit(AssignStmt* stmt) {
    if (!table->lookup(stmt->lhs)) {
        std::cerr << "Name resolution error: undefined variable '"
                  << stmt->lhs << "'\n";
    }
    stmt->rhs->accept(this);
}

void NameRes::visit(ForStmt* stmt) {
    stmt->start->accept(this);
    stmt->end->accept(this);
    table->pushScope();
    table->declare(stmt->id, {false, Var::I32, 0, ""});
    stmt->block->accept(this);
    table->popScope();
}

void NameRes::visit(WhileStmt* stmt) {
    stmt->cond->accept(this);
    stmt->block->accept(this);
}

void NameRes::visit(PrintStmt* stmt) {
    for (auto exp : stmt->args) {
        exp->accept(this);
    }
}

void NameRes::visit(BreakStmt* stmt) {
    if (stmt->exp) {
        stmt->exp->accept(this);
    }
}

void NameRes::visit(ReturnStmt* stmt) {
    if (stmt->exp) {
        stmt->exp->accept(this);
    }
}

void NameRes::visit(ExpStmt* stmt) {
    if (stmt->exp) {
        stmt->exp->accept(this);
    }
}

void NameRes::visit(Fun* fun) {
    table->pushScope();
    for (auto& p : fun->params) {
        table->declare(p.id, {false, p.type, 0, ""});
    }
    fun->block->accept(this);
    table->popScope();
}

void NameRes::visit(Program* program) {
    table->pushScope();
    for (const auto &id: program->funs | std::views::keys) {
        table->declare(id, {false, Var::VOID, 0, ""});
    }
    for (const auto &fun: program->funs | std::views::values) {
        fun->accept(this);
    }
    table->popScope();
}
