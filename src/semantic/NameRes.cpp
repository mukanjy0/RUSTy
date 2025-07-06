#include "NameRes.h"
#include <iostream>
#include <ranges>
#include <string>

NameRes::~NameRes() = default;

void NameRes::declare(const std::string& id, const Value& val, int line, int col) const {
    if (!table->declare(id, val)) {
        throw std::runtime_error("redeclaration on the same scope " + std::to_string(line) + ':' + std::to_string(col));
    }
}

void NameRes::update(const std::string& id, const Value& val, int line, int col) const {
    if (!table->update(id, val)) {
        throw std::runtime_error("update of undefined identifier '" + id + "' at " + std::to_string(line) + ':' + std::to_string(col));
    }
}

Value NameRes::lookup(const std::string& id, int line, int col) const {
    if (auto val = table->lookup(id); val) {
        return *val;
    } throw std::runtime_error("undefined identifier '" + id + "' at " + std::to_string(line) + ':' + std::to_string(col));
}

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
    lookup(exp->name, exp->line, exp->col);
    return {Value::ID, exp->name};
}

Value NameRes::visit(FunCall* exp) {
    if (auto val = lookup(exp->id, exp->line, exp->col); !val.isFunction()) {
        throw std::runtime_error(
            "‘" + exp->id + "’ is not a function at " +
            std::to_string(exp->line) + ":" +
            std::to_string(exp->col));
    }
    for (auto* arg : exp->args)
        arg->accept(this);
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
    lookup(exp->id, exp->line, exp->col);
    exp->exp->accept(this);
    return {Value::ID, exp->id};
}

Value NameRes::visit(SliceExp* exp) {
    lookup(exp->id, exp->line, exp->col);
    if (exp->start) exp->start->accept(this);
    if (exp->end) exp->end->accept(this);
    return {Value::ID, exp->id};
}

Value NameRes::visit(ReferenceExp* exp) {
    exp->exp->accept(this);
    return {};
}

Value NameRes::visit(ArrayExp* exp) {
    for (auto el : exp->elements) {
        el->accept(this);
    }
    return {};
}

Value NameRes::visit(UniformArrayExp* exp) {
    exp->value->accept(this);
    exp->size->accept(this);
    return {};
}

Value NameRes::visit(DecStmt* stmt) {
    declare(stmt->id, stmt->var, stmt->line, stmt->col);
    if (stmt->rhs) {
        stmt->rhs->accept(this);
    }
    return {};
}

Value NameRes::visit(AssignStmt* stmt) {
    stmt->lhs->accept(this);
    stmt->rhs->accept(this);
    return {};
}

Value NameRes::visit(CompoundAssignStmt* stmt) {
    stmt->lhs->accept(this);
    stmt->rhs->accept(this);
    return {};
}

Value NameRes::visit(ForStmt* stmt) {
    stmt->start->accept(this);
    stmt->end->accept(this);
    table->pushScope();
    Value value {};
    value.initialized = true;
    declare(stmt->id, value, stmt->line, stmt->col);
    stmt->block->accept(this);
    table->popScope();
    return {};
}

Value NameRes::visit(WhileStmt* stmt) {
    stmt->cond->accept(this);
    stmt->block->accept(this);
    return {};
}

Value NameRes::visit(PrintStmt* stmt) {
    for (auto exp : stmt->args) {
        exp->accept(this);
    }
    return {};
}

Value NameRes::visit(BreakStmt* stmt) {
    if (stmt->exp) stmt->exp->accept(this);
    return {};
}

Value NameRes::visit(ReturnStmt* stmt) {
    if (stmt->exp) stmt->exp->accept(this);
    return {};
}

Value NameRes::visit(ExpStmt* stmt) {
    stmt->exp->accept(this);
    return {};
}

Value NameRes::visit(Fun* fun) {
    table->pushScope();
    for (auto& param : fun->params) {
        Value val; val.type=param.type;
        declare(param.id, val, param.line, param.col);
    }
    fun->block->accept(this);
    table->popScope();
    return {};
}

void NameRes::visit(Program* program) {
    table->pushScope();
    for (const auto& [id, fun]: program->funs) {
        Value val(fun->type);
        val.fun = true;
        for (const auto& param : fun->params)
            val.addType(param.type);
        declare(id, val, fun->line, fun->col);
    }
    for (const auto &fun: program->funs | std::views::values) {
        fun->accept(this);
    }
    table->popScope();
}
