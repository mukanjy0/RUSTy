#include "NameRes.h"
#include <iostream>

NameRes::~NameRes() = default;

Value NameRes::visit(Block* block) {
    table->pushScope();
    for (auto stmt : block->stmts) {
        stmt->accept(this);
    }
    table->popScope();
    return {};
}

Value NameRes::visit(BinaryExp* exp) {
    exp->lhs->accept(this);
    exp->rhs->accept(this);
    return {};
}

Value NameRes::visit(UnaryExp* exp) {
    exp->exp->accept(this);
    return {};
}

Value NameRes::visit(Literal* exp) {
    (void)exp; // unused
    return {};
}

Value NameRes::visit(Variable* exp) {
    if (!table->lookup(exp->name)) {
        std::cerr << "Name resolution error: undefined variable '"
                  << exp->name << "'\n";
    }
    return {};
}

Value NameRes::visit(FunCall* exp) {
    for (auto arg : exp->args) {
        arg->accept(this);
    }
    return {};
}

Value NameRes::visit(IfExp* exp) {
    exp->ifBranch->getCondition()->accept(this);
    exp->ifBranch->getBlock()->accept(this);
    for (auto& br : exp->elseIfBranches) {
        br->getCondition()->accept(this);
        br->getBlock()->accept(this);
    }
    if (exp->elseBranch) {
        exp->elseBranch->getBlock()->accept(this);
    }
    return {};
}

Value NameRes::visit(LoopExp* exp) {
    exp->block->accept(this);
    return {};
}

Value NameRes::visit(SubscriptExp* exp) {
    // Implementation here
    return {};
}

Value NameRes::visit(SliceExp* exp) {
    // Implementation here
    return {};
}

Value NameRes::visit(ReferenceExp* exp) {
    // Implementation here
    return {};
}

Value NameRes::visit(ArrayExp* exp) {
    // std::cout << exp;
    return {};
}

Value NameRes::visit(UniformArrayExp* exp) {
    // std::cout << exp;
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
    stmt->lhs->accept(this);
    stmt->rhs->accept(this);
}

void NameRes::visit(CompoundAssignStmt* stmt) {
    stmt->lhs->accept(this);
    stmt->rhs->accept(this);
}

void NameRes::visit(ForStmt* stmt) {
    stmt->start->accept(this);
    stmt->end->accept(this);
    table->pushScope();
    table->declare(stmt->id, {});
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
    stmt->exp->accept(this);
}

void NameRes::visit(ReturnStmt* stmt) {
    stmt->exp->accept(this);
}

void NameRes::visit(ExpStmt* stmt) {
    stmt->exp->accept(this);
}

void NameRes::visit(Fun* fun) {
    table->pushScope();
    for (auto& param : fun->params) {
        Value val; val.type=param.type;
        if (!table->declare(param.id, val))
            throw std::runtime_error("redeclaration on the same scope" + fun->line + ':' + fun->col);
    }
    fun->block->accept(this);
    table->popScope();
}

void NameRes::visit(Program* program) {
    for (const auto& [id, fun]: program->funs) {
        Value val{fun->type, true};
        for (const auto& param : fun->params)
            val.addType(param.type);
        if (!table->declare(id, val))
            throw std::runtime_error("redeclaration on the same scope" + fun->line + ':' + fun->col);
    }
    for (const auto& [id, fun]: program->funs) {
        fun->accept(this);
    }
}
