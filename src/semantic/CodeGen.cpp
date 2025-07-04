#include "CodeGen.h"

CodeGen::Operand::operator string() const {
    string res {};
    if (mem) res += to_string(offset) + "(";
    if (constant) {
        if (!mem) res += "$";
        res += to_string(value);
    }
    else {
        res += "%";
        switch (lvl) {
            case B:
                res += reg + "l";
                break;
            case W:
                res += reg;
                if (reg.length() == 1) res += "x";
            case D:
                res += "e" + reg;
                if (reg.length() == 1) res += "x";
            case Q:
                res += "r" + reg;
                if (reg.length() == 1) res += "x";
        }
    }
    if (mem) res += ")";
    return res;
}

char CodeGen::suffix(L lvl) {
    switch (lvl) {
        case B: return 'b';
        case W: return 'w';
        case D: return 'l';
        case Q: return 'q';
    }
}
char CodeGen::suffix(CodeGen::Operand o) {
    return suffix(o.lvl);
}
char CodeGen::suffix(Value value) {
    return suffix(valueToL(value));
}
char CodeGen::suffix(Value::Type type) {
    return suffix(typeToL(type));
}

CodeGen::L CodeGen::typeToL(Value::Type type) {
    switch(type) {
        case Value::CHAR: 
        case Value::BOOL: return B;
        case Value::I32: 
        case Value::STR: return D; // it will always be a pointer
        default: return D;
    }
}

CodeGen::L CodeGen::valueToL(Value value) {
    if (value.ref) return D;

    switch(value.type) {
        case Value::CHAR: 
        case Value::BOOL: return B;
        case Value::I32: 
        case Value::STR: return D; // it will always be a pointer
        default: return D;
    }
}

void CodeGen::mov(Operand l, Operand r) {
    out << "mov" << suffix(l) << ' ' << l << ", " << r << '\n';
}
void CodeGen::movz(Operand l, Operand r) {
    out << "movz" << suffix(l) << suffix(r) << ' ' << l << ", " << r << '\n';
}
void CodeGen::add(Operand l, Operand r) {
    out << "add" << suffix(l) << ' ' << l << ", " << r << '\n';
}
void CodeGen::inc(Operand r) {
    out << "inc" << suffix(r) << ' ' << r << '\n';
}
void CodeGen::sub(Operand l, Operand r) {
    out << "sub" << suffix(l) << ' ' << l << ", " << r << '\n';
}
void CodeGen::dec(Operand r) {
    out << "dec" << suffix(r) << ' ' << r << '\n';
}
void CodeGen::mul(Operand l, Operand r) {
    out << "imul" << suffix(l) << ' ' << l << ", " << r << '\n';
}
void CodeGen::div(Operand l, Operand r) {
    out << "idiv" << suffix(l) << ' ' << l << ", " << r << '\n';
}
void CodeGen::push(Operand r) {
    out << "push" << suffix(r) << ' ' << r << '\n';
}
void CodeGen::pop(Operand r) {
    out << "pop" << suffix(r) << ' ' << r << '\n';
}
void CodeGen::lea(Operand l, Operand r) {
    if (!l.mem) {
        throw runtime_error("expected <mem> left operand for lea");
    }
    out << "lea" << suffix(l) << ' ' << l << ", " << r << '\n';
}
void CodeGen::cmp(Operand l, Operand r) {
    out << "cmp" << suffix(l) << ' ' << r << '\n';
}
void CodeGen::jmp(Operand r, C cond) {
    if (cond!=NONE) {
        out << "j" << cond;
    }
    else {
        out << "jmp";
    }
    out << ' ' << r << '\n';
}
void CodeGen::set(Operand r, C cond) {
    out << "set" << cond << ' ' << r << '\n';
}
void CodeGen::cmov(Operand l, Operand r, C cond) {
    out << "cmov" << cond << ' ' << l << ' ' << r << '\n';
}

// Destructor
CodeGen::~CodeGen() = default;

// Visit methods for expressions
Value CodeGen::visit(Block* block) {
    table->pushScope();
    Value val;
    for(auto stmt : block->stmts) {
        val = stmt->accept(this);
    }
    table->popScope();
    return {};
}

Value CodeGen::visit(BinaryExp* exp) {
    exp->lhs->accept(this);
    push({});
    exp->rhs->accept(this);
    mov({}, {"c"});
    pop({});

    switch (exp->op) {
        case BinaryExp::LAND:
            out << " andq %rcx, %rax\n";
            break;
        case BinaryExp::LOR:
            out << " orq %rcx, %rax\n";
            break;
        case BinaryExp::GT:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setg %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::LT:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setl %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::GE:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setge %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::LE:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setle %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::EQ:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " sete %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::NEQ:
            out << " cmpq %rcx, %rax\n"
                 << " movl $0, %eax\n"
                 << " setne %al\n"
                 << " movzbq %al, %rax\n";
            break;
        case BinaryExp::PLUS:
            out << " addq %rcx, %rax\n";
            break;
        case BinaryExp::MINUS:
            out << " subq %rcx, %rax\n";
            break;
        case BinaryExp::TIMES:
            out << " imulq %rcx, %rax\n";
            break;
        case BinaryExp::DIV:
            out  << " cqto\n"
                 << " idivq %rcx\n";
            break;
        default:
            throw std::runtime_error("Invalid binary operation");
    }
    return {};

}

Value CodeGen::visit(UnaryExp* exp) {
    exp->exp->accept(this);
    mov({}, {"c"});

    switch (exp->op) {
        case UnaryExp::LNOT:
            out << " notq %rcx\n";
            break;
        default:
            throw std::runtime_error("Invalid unary operation");
    }

    mov({"c"}, {});
    return {};
}

Value CodeGen::visit(Literal* exp) {
    left.value = exp->value;
    left.constant = true;
    out << " movq $" << exp->value << ", %rax\n";
    return {};
}

Value CodeGen::visit(Variable* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(FunCall* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(IfExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(LoopExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(SubscriptExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(SliceExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(ReferenceExp* exp) {
    // Implementation here
    return {};
}

Value CodeGen::visit(ArrayExp* exp) {
    std::cout << exp;
    return {};
}

Value CodeGen::visit(UniformArrayExp* exp) {
    std::cout << exp;
    return {};
}

// Visit methods for statements
void CodeGen::visit(DecStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(AssignStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(CompoundAssignStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(ForStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(WhileStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(PrintStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(BreakStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(ReturnStmt* stmt) {
    // Implementation here
}

void CodeGen::visit(ExpStmt* stmt) {
    // Implementation here
}

// Visit methods for functions and programs
void CodeGen::visit(Fun* fun) {
    push({"bp"});
    mov({"sp"}, {"bp"});
    int len = fun->params.size();
    for (auto param : fun->params) {
        value.addType(param.type);
    }
    pop({"bp"});
}

void CodeGen::visit(Program* program) {
    table->pushScope();
    table->popScope();
    for (auto [id, fun] : program->funs) {
        Value value (Value::ID, id);
        value.fun = true;

        for (auto param : fun->params) {
            value.addType(param.type);
        }

        table->declare(id, value);

        out << ".text\n";
        out << ".globl " << id << '\n';
        out << ".type " << id << ", @function";
        out << id << ":\n";
        fun->accept(this);
    }
    table->popScope();
    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
}
