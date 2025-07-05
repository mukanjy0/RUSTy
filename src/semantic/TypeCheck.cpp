#include "TypeCheck.h"
#include <ranges>


Value TypeCheck::lookup(const std::string& id) const {
    return *table->lookup(id);
}

void TypeCheck::declare(const std::string& id, const Value& val) const {
        table->declare(id, val);
}

int TypeCheck::getScopeDepth() const {
    return table->getScopeDepth();
}

void TypeCheck::assertMut(const Value& val, int line, int col) {
    if (!val.mut) {
        throw std::runtime_error("cannot assign to immutable variable at "
                                + std::to_string(line) + ':' + std::to_string(col));
    }
}

void TypeCheck::assertType(Value::Type from, Value::Type to, int line, int col) {
    if (from != to) {
        throw std::runtime_error("type mismatch at " + std::to_string(line) +
                                 ':' + std::to_string(col));
    }
}

TypeCheck::~TypeCheck() = default;

// Visit methods for expressions
Value TypeCheck::visit(Block* block) {
    table->pushScope();
    Value::Type last = Value::UNIT;
    for (auto stmt : block->stmts) {
        last = stmt->accept(this).type;
    }
    table->popScope();
    block->type = last;
    return {last};
}

Value TypeCheck::visit(BinaryExp* exp) {
    Value lhs = exp->lhs->accept(this);
    Value rhs = exp->rhs->accept(this);
    switch (exp->op) {
        case BinaryExp::LAND:
        case BinaryExp::LOR:
            assertType(lhs.type, Value::BOOL, exp->line, exp->col);
            assertType(rhs.type, Value::BOOL, exp->line, exp->col);
            exp->type = Value::BOOL;
            break;
        case BinaryExp::GT: case BinaryExp::LT:
        case BinaryExp::GE: case BinaryExp::LE:
            assertType(lhs.type, Value::I32, exp->line, exp->col);
            assertType(rhs.type, Value::I32, exp->line, exp->col);
            exp->type = Value::BOOL;
            break;
        case BinaryExp::EQ: case BinaryExp::NEQ:
            assertType(lhs.type, rhs.type, exp->line, exp->col);
            if (lhs.type != Value::I32 && lhs.type != Value::BOOL)
                throw std::runtime_error("Invalid binary operation");
            exp->type = Value::BOOL;
            break;
        case BinaryExp::PLUS: case BinaryExp::MINUS:
        case BinaryExp::TIMES: case BinaryExp::DIV:
            assertType(lhs.type, Value::I32, exp->line, exp->col);
            assertType(rhs.type, Value::I32, exp->line, exp->col);
            exp->type = Value::I32;
            break;
        default:
            throw std::runtime_error("Invalid binary operation");
    }
    return {exp->type};

}

Value TypeCheck::visit(UnaryExp* exp) {
    Value v = exp->exp->accept(this);
    switch (exp->op) {
        case UnaryExp::LNOT:
            assertType(v.type, Value::BOOL, exp->line, exp->col);
            exp->type = Value::BOOL;
            break;
        default:
            throw std::runtime_error("Invalid unary operation");
    }
    return {exp->type};
}

Value TypeCheck::visit(Literal* exp) {
    return exp->value;
}

Value TypeCheck::visit(Variable* exp) {
    Value v = lookup(exp->name);
    if (!lhsContext && !v.initialized)
        throw std::runtime_error("use of uninitialized variable at " +
                                 std::to_string(exp->line) + ':' +
                                 std::to_string(exp->col));
    exp->type = v.type;
    return v;
}

Value TypeCheck::visit(FunCall* exp) {
    Value fn = lookup(exp->id);
    if (fn.types.size() != exp->args.size())
        throw std::runtime_error("incorrect argument count at " +
                                 std::to_string(exp->line) + ':' +
                                 std::to_string(exp->col));
    auto it = fn.types.begin();
    for (auto arg : exp->args) {
        Value a = arg->accept(this);
        assertType(a.type, *it, arg->line, arg->col);
        ++it;
    }
    exp->type = fn.type;
    return {fn.type};
}

Value TypeCheck::visit(IfExp* exp) {
    Value cond = exp->ifBranch->getCondition()->accept(this);
    Exp* condex = exp->ifBranch->getCondition();
    assertType(cond.type, Value::BOOL, condex->line, condex->col);
    Value t = exp->ifBranch->getBlock()->accept(this);
    Value e = {Value::UNIT};
    for (auto br : exp->elseIfBranches) {
        Value c = br->getCondition()->accept(this);
        Exp* cex = br->getCondition();
        assertType(c.type, Value::BOOL, cex->line, cex->col);
        e = br->getBlock()->accept(this);
    }
    if (exp->elseBranch)
        e = exp->elseBranch->getBlock()->accept(this);
    assertType(t.type, e.type, exp->line, exp->col);
    exp->type = t.type;
    return {exp->type};
}

Value TypeCheck::visit(LoopExp* exp) {
    Value v = exp->block->accept(this);
    exp->type = v.type;
    return v;
}

Value TypeCheck::visit(SubscriptExp* exp) {
    Value coll = lookup(exp->id);
    Value idx = exp->exp->accept(this);
    assertType(idx.type, Value::I32, exp->line, exp->col);
    if (!(coll.type == Value::STR || coll.size > 0))
        throw std::runtime_error("subscript on non-indexable at " +
                                 std::to_string(exp->line) + ':' +
                                 std::to_string(exp->col));
    exp->type = coll.type;
    return lookup(exp->id);
}

Value TypeCheck::visit(SliceExp* exp) {
    lookup(exp->id);
    if (exp->start) {
        Value s = exp->start->accept(this);
        Exp* ex = exp->start;
        assertType(s.type, Value::I32, ex->line, ex->col);
    }
    if (exp->end) {
        Value e = exp->end->accept(this);
        Exp* ex = exp->end;
        assertType(e.type, Value::I32, ex->line, ex->col);
    }
    exp->type = Value::STR;
    return {exp->type};
}

Value TypeCheck::visit(ReferenceExp* exp) {
    Value v = exp->exp->accept(this);
    exp->type = v.type;
    return v;
}

Value TypeCheck::visit(ArrayExp* exp) {
    Value::Type elType = Value::UNDEFINED;
    for (auto el : exp->elements) {
        Value v = el->accept(this);
        if (elType == Value::UNDEFINED) elType = v.type;
        else assertType(v.type, elType, el->line, el->col);
    }
    exp->type = elType;
    Value val{elType};
    val.size = exp->elements.size();
    return val;
}

Value TypeCheck::visit(UniformArrayExp* exp) {
    Value v = exp->value->accept(this);
    Value s = exp->size->accept(this);
    Exp* size = exp->size;
    assertType(s.type, Value::I32, size->line, size->col);
    exp->type = v.type;
    Value val{v.type};
    val.size = s.numericValues.front();
    return val;
}

// Visit methods for statements
Value TypeCheck::visit(DecStmt* stmt) {
    Value rhs{stmt->var.type};
    if (stmt->rhs) {
        rhs = stmt->rhs->accept(this);
        if (stmt->var.type == Value::UNDEFINED)
            stmt->var.type = rhs.type;
        assertType(rhs.type, stmt->var.type, stmt->line, stmt->col);
        stmt->var.initialized = true;
    } else {
        stmt->var.initialized = false;
    }
    declare(stmt->id, stmt->var);
    return stmt->var;
}

Value TypeCheck::visit(AssignStmt* stmt) {
    lhsContext = true;
    Value lhs = stmt->lhs->accept(this);
    lhsContext = false;
    Value rhs = stmt->rhs->accept(this);
    if (auto var = dynamic_cast<Variable*>(stmt->lhs)) {
        Value* entry = table->lookup(var->name);
        if (!entry->initialized) {
            if (entry->type == Value::UNDEFINED)
                entry->type = rhs.type;
            if (rhs.size > 0)
                entry->size = rhs.size;
            entry->initialized = true;
        } else {
            assertMut(*entry, stmt->line, stmt->col);
            assertType(rhs.type, entry->type, stmt->line, stmt->col);
        }
    } else {
        assertMut(lhs, stmt->line, stmt->col);
        assertType(rhs.type, lhs.type, stmt->line, stmt->col);
    }
    return {Value::UNIT};
}

Value TypeCheck::visit(CompoundAssignStmt* stmt) {
    Value lhs = stmt->lhs->accept(this);
    Value rhs = stmt->rhs->accept(this);
    assertMut(lhs, stmt->line, stmt->col);
    assertType(lhs.type, Value::I32, stmt->line, stmt->col);
    assertType(rhs.type, Value::I32, stmt->line, stmt->col);
    return {Value::UNIT};
}

Value TypeCheck::visit(ForStmt* stmt) {
    stmt->start->accept(this);
    stmt->end->accept(this);
    table->pushScope();
    declare(stmt->id, Value{Value::I32});
    stmt->block->accept(this);
    table->popScope();
    return {Value::UNIT};
}


Value TypeCheck::visit(WhileStmt* stmt) {
    Value c = stmt->cond->accept(this);
    Exp* cond = stmt->cond;
    assertType(c.type, Value::BOOL, cond->line, cond->col);
    table->pushScope();
    stmt->block->accept(this);
    table->popScope();
    return {Value::UNIT};
}

Value TypeCheck::visit(PrintStmt* stmt) {
    for (auto exp : stmt->args) exp->accept(this);
    return {Value::UNIT};
}

Value TypeCheck::visit(BreakStmt* stmt) {
    if (getScopeDepth() <= scopeDepth)
        throw std::runtime_error("break outside loop at " +
                                 std::to_string(stmt->line) + ':' +
                                 std::to_string(stmt->col));
    Value r{Value::UNIT};
    if (stmt->exp) r = stmt->exp->accept(this);
    stmt->type = r.type;
    return r;
}

Value TypeCheck::visit(ReturnStmt* stmt) {
    Value r{Value::UNIT};
    if (stmt->exp) r = stmt->exp->accept(this);
    assertType(r.type, currentReturnType, stmt->line, stmt->col);
    stmt->type = r.type;
    return r;
}

Value TypeCheck::visit(ExpStmt* stmt) {
    Value r = stmt->exp->accept(this);
    stmt->type = stmt->returnValue ? r.type : Value::UNIT;
    return {stmt->type};
}

// Visit methods for functions and programs
Value TypeCheck::visit(Fun* fun) {
    scopeDepth = getScopeDepth();
    table->pushScope();
    currentReturnType = fun->type != Value::UNDEFINED ? fun->type : Value::UNIT;
    for (const auto& p : fun->params) {
        declare(p.id, Value{p.type});
    }
    Value r = fun->block->accept(this);
    if (fun->type == Value::UNDEFINED)
        fun->type = r.type;
    table->popScope();
    return {Value::UNIT};
}

void TypeCheck::visit(Program* program) {
    table->pushScope();
    for (const auto& [id, fun]: program->funs) {
        Value val{fun->type, true};
        for (const auto& param : fun->params)
            val.addType(param.type);
        declare(id, val);
    }
    for (const auto &[id, fun]: program->funs) {
        fun->accept(this);
        Value* entry = table->lookup(id);
        if (entry) entry->type = fun->type != Value::UNDEFINED ? fun->type : Value::UNIT;
    }
    table->popScope();
}