#include "CodeGen.h"

CodeGen::Operand::operator string() const {
    string res {};
    if (mem) res += to_string(offset) + "(";
    if (constant) res += value;
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

void CodeGen::mov(Value value, Operand l, Operand r) {
    out << "mov" << suffix(value) << ' ' << l << ", " << r << '\n';
}
void CodeGen::add(Value value, Operand l, Operand r) {
    out << "add" << suffix(value) << ' ' << l << ", " << r << '\n';
}
void CodeGen::inc(Value value, Operand o) {
    out << "inc" << suffix(value) << ' ' << o << '\n';
}
void CodeGen::sub(Value value, Operand l, Operand r) {
    out << "sub" << suffix(value) << ' ' << l << ", " << r << '\n';
}
void CodeGen::dec(Value value, Operand o) {
    out << "dec" << suffix(value) << ' ' << o << '\n';
}
void CodeGen::mul(Value value, Operand l, Operand r) {
    out << "imul" << suffix(value) << ' ' << l << ", " << r << '\n';
}
void CodeGen::div(Value value, Operand l, Operand r) {
    out << "idiv" << suffix(value) << ' ' << l << ", " << r << '\n';
}
void CodeGen::push(Value value, Operand o) {
    out << "push" << suffix(value) << ' ' << o << '\n';
}
void CodeGen::pop(Value value, Operand o) {
    out << "pop" << suffix(value) << ' ' << o << '\n';
}
void CodeGen::lea(Value value, Operand l, Operand r) {
    if (!l.mem) {
        throw runtime_error("expected <mem> left operand for lea");
    }
    out << "lea" << suffix(value) << ' ' << l << ", " << r << '\n';
}

// Destructor
CodeGen::~CodeGen() = default;

// Visit methods for expressions
Value CodeGen::visit(Block* block) {
    for(auto stmt : block->stmts) {
        stmt->accept(this);
    }
    return {};
}

Value CodeGen::visit(BinaryExp* exp) {
    exp->lhs->accept(this);
    out << " pushq %rax\n";
    exp->rhs->accept(this);
    out << " movq %rax, %rcx\n";
    out << " popq %rax\n";

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
    out << " movq %rax, %rcx\n";

    switch (exp->op) {
        case UnaryExp::LNOT:
            out << " notq %rcx\n";
            break;
        default:
            throw std::runtime_error("Invalid unary operation");
    }

    out << " movq %rcx, %rax\n";
    return {};
}

Value CodeGen::visit(Literal* exp) {
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
    // Implementation here
}

void CodeGen::visit(Program* program) {
    // Implementation here
}
