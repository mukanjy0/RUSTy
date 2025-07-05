#include "CodeGen.h"

Operand::~Operand() = default;
std::ostream& operator<<(std::ostream& out, Operand* op) {
    op->print(out);
    return out;
}

Reg::~Reg() {}
void Reg::print(ostream& out) {
    out << "%";
    switch (lvl) {
        case B:
            out << reg + "l";
            break;
        case W:
            out << reg;
            if (reg.length() == 1) out << "x";
        case D:
            out << "e" + reg;
            if (reg.length() == 1) out << "x";
        case Q:
            out << "r" + reg;
            if (reg.length() == 1) out << "x";
    }
}

Const::~Const() {}
void Const::print(ostream& out) {
    out << "$";
    out << value.numericValues.front();
}

Mem::~Mem() {
    delete reg;
}
void Mem::print(ostream& out) {
    if (offset) out << to_string(offset);
    else if (!label.empty()) out << label;
    out << "(";
    out << reg;
    out << ")";
}

ostream& operator<<(ostream& out, L lvl) {
    switch (lvl) {
        case B: out << 'b';
        case W: out << 'w';
        case D: out << 'l';
        case Q: out << 'q';
    }
    return out;
}

ostream& operator<<(ostream& out, C cond) {
    switch (cond) {
        case EQ: out << "e"; break;
        case NE: out << "ne"; break;
        case GT: out << "g"; break;
        case LT: out << "l"; break;
        case GE: out << "ge"; break;
        case LE: out << "le"; break;
        default: break;
    }
    return out;
}

int CodeGen::typeLen(L lvl) {
    switch (lvl) {
        case B: return 1;
        case W: return 2;
        case D: return 4;
        case Q: return 8;
        default: return 8;
    }
}

int CodeGen::typeLen(Value::Type type) {
    switch(type) {
        case Value::BOOL:
        case Value::CHAR: return 1;
        case Value::I32: return 4;
        default: return 8;
    }
}

L CodeGen::typeToL(Value::Type type) {
    switch(type) {
        case Value::CHAR: 
        case Value::BOOL: return B;
        case Value::I32: 
        case Value::UNIT: return D;
        case Value::STR: return Q; // it will always be a pointer
        default: return Q;
    }
}

L CodeGen::valueToL(Value value) {
    if (value.ref) return Q;

    switch(value.type) {
        case Value::CHAR: 
        case Value::BOOL: return B;
        case Value::I32: 
        case Value::UNIT: return D;
        case Value::STR: return Q; // it will always be a pointer
        default: return Q;
    }
}

void CodeGen::mov() {
    out << "mov" << l->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::movz() {
    out << "movz" << l->lvl << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::add() {
    out << "add" << l->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::inc() {
    out << "inc" << r->lvl << ' ' << r << '\n';
}
void CodeGen::sub() {
    out << "sub" << l->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::dec() {
    out << "dec" << r->lvl << ' ' << r << '\n';
}
void CodeGen::mul() {
    out << "imul" << l->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::div() {
    out << "idiv" << l->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::push() {
    out << "push" << r->lvl << ' ' << r << '\n';
    offset += typeLen(r->lvl);
}
void CodeGen::pop() {
    out << "pop" << r->lvl << ' ' << r << '\n';
    offset -= typeLen(r->lvl);
}
void CodeGen::lea() {
    out << "lea" << l->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::cmp() {
    out << "cmp" << l->lvl << ' ' << r << '\n';
}
void CodeGen::jmp(C cond) {
    if (cond!=NONE) {
        out << "j" << cond;
    }
    else {
        out << "jmp";
    }
    out << ' ' << r << '\n';
}
void CodeGen::jmp(string label, C cond) {
    if (cond!=NONE) {
        out << "j" << cond;
    }
    else {
        out << "jmp";
    }
    out << ' ' << label << '\n';
}
void CodeGen::set(C cond) {
    out << "set" << cond << ' ' << r << '\n';
}
void CodeGen::cmov(C cond) {
    out << "cmov" << cond << ' ' << l << ' ' << r << '\n';
}
void CodeGen::call(string label) {
    out << "call " << label << "\n";
    offset += typeLen(Q);
}
void CodeGen::enter() {
    r = new Reg("bp");
    push();

    l = new Reg("sp");
    r = new Reg("bp");
    mov();

    bp.push(offset);
}
void CodeGen::leave() {
    out << "leave\n";
    offset -= typeLen(Q);

    bp.pop();
}
void CodeGen::ret() {
    out << "ret\n";
    offset -= typeLen(Q);
}
string CodeGen::LCLabel() {
    string label = ".LC" + to_string(++lc);
    out << label << '\n';
    return label;
}
void CodeGen::LBLabel() {
    string label = ".LB" + to_string(++lb);
    out << label << '\n';
    lbs.push(lb);
    labels.push(label);
}
void CodeGen::LELabel() {
    string label = ".LE" + to_string(lbs.top());
    out << label << '\n';
    lbs.pop();
    labels.pop();
}
void CodeGen::LFBLabel() {
    string label = ".LFB" + to_string(++lf);
    out << label << '\n';
    labels.push(label);
}
void CodeGen::LFELabel() {
    string label = ".LFE" + to_string(lf);
    out << label << '\n';
    labels.pop();
}
string CodeGen::end(string label) {
    if (label[1] == 'F') {
        label[2] = 'E';
    }
    else {
        label[1] = 'E';
    }
    return label;
}

// Destructor
CodeGen::~CodeGen() = default;

// Visit methods for expressions
Value CodeGen::visit(Block* block) {
    if (init) {
        table->pushScope();
        Value val;
        cur.push(block);
        for(auto stmt : block->stmts) {
            val = stmt->accept(this);
        }
        cur.pop();
        table->popScope();
    }
    else {
        cur.push(block);
        for(auto stmt : block->stmts) {
            stmt->accept(this);
        }
        cur.pop();
    }
    return {};
}

Value CodeGen::visit(BinaryExp* exp) {
    if (init) {
        exp->lhs->accept(this);

        r = new Reg();
        push();

        exp->rhs->accept(this);

        l = new Reg();
        r = new Reg("c");
        mov();

        r = new Reg();
        pop();

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
    }
    else {
        exp->lhs->accept(this);
        exp->rhs->accept(this);
        return {};
    }

}

Value CodeGen::visit(UnaryExp* exp) {
    if (init) {
        exp->exp->accept(this);

        l = new Reg();
        r = new Reg("c");
        mov();

        switch (exp->op) {
            case UnaryExp::LNOT:
                out << " notq %rcx\n";
                break;
            default:
                throw std::runtime_error("Invalid unary operation");
        }

        l = new Reg("c");
        r = new Reg();
        mov();
    }
    else {
        exp->exp->accept(this);
        return {};
    }
}

Value CodeGen::visit(Literal* exp) {
    if (init) {
        out << " movq $" << exp->value << ", %rax\n";
    }
    else {
        if (exp->value.type == Value::STR) {
            string label = ".LC" + to_string(++lc);
            out << label << '\n';
            out << ".string \"" << exp->value.stringValues.front() << "\"\n"; 

            exp->value.type = Value::ID;
            exp->value.stringValues.front() = label;
        }
    }
    return {};
}

Value CodeGen::visit(Variable* exp) {
    if (init) {
    }
    return {};
}

Value CodeGen::visit(FunCall* exp) {
    if (init) {
        stack<Exp*> s;
        for (auto arg : exp->args) {
            s.push(arg);
        }

        while (!s.empty()) {
            auto arg = s.top();
            s.pop();

            Value value = arg->accept(this);
            offset += typeLen(value.type);

            r = new Reg();
            r->lvl = valueToL(value);
            push();
        }

        prevOff = offset;
        call(exp->id);
    }
    else {
        for (auto arg : exp->args) {
            arg->accept(this);
        }
    }
    return {};
}

Value CodeGen::visit(IfExp* exp) {
    if (init) {
    }
    else {
        exp->ifBranch->block->accept(this);
        for (auto branch : exp->elseIfBranches) {
            branch->block->accept(this);
        }
        if (exp->elseBranch) exp->elseBranch->block->accept(this);
    }
    return {};
}

Value CodeGen::visit(LoopExp* exp) {
    if (init) {
    }
    else {
        exp->block->accept(this);
    }
    return {};
}

Value CodeGen::visit(SubscriptExp* exp) {
    if (init) {
    }
    return {};
}

Value CodeGen::visit(SliceExp* exp) {
    if (init) {
    }
    return {};
}

Value CodeGen::visit(ReferenceExp* exp) {
    if (init) {
    }
    else {
        exp->exp->accept(this);
    }
    return {};
}

Value CodeGen::visit(ArrayExp* exp) {
    if (init) {

    }
    else {
        for (auto el : exp->elements) {
            el->accept(this);
        }
    }
    return {};
}

Value CodeGen::visit(UniformArrayExp* exp) {
    if (init) {

    }
    else {
        exp->value->accept(this);
    }
    return {};
}

// Visit methods for statements
Value CodeGen::visit(DecStmt* stmt) {
    if (init) {
        Block* b = cur.top();
        allocated[b] += typeLen(stmt->var.type);
        int off = toAllocate[b] - allocated[b];
        table->declare(stmt->id, Value(stmt->var.type, off));

        if (stmt->rhs) {
            auto rhs = stmt->rhs->accept(this);

            l = new Reg();
            auto reg = new Reg("bp");
            r = new Mem(reg, off);
            mov();
        }
        return Value(Value::UNIT, 0);
    }
    else {
        toAllocate[cur.top()] += typeLen(stmt->var.type);
        if (stmt->var.type == Value::STR) {
            stmt->rhs->accept(this);
        }
        return {};
    }
}

Value CodeGen::visit(AssignStmt* stmt) {
    if (init) {
        auto lhs = stmt->lhs->accept(this);
        r = new Reg();
        push();

        auto rhs = stmt->rhs->accept(this);

        r = new Reg("c");
        pop();

        l = new Reg();
        auto reg = new Reg("c");
        r = new Mem(reg, 0);
        mov();
        return Value(Value::UNIT, 0);
    }
    else {
        stmt->rhs->accept(this);
        return {};
    }
}

Value CodeGen::visit(CompoundAssignStmt* stmt) {
    if (init) {
    }
    return {};
}

Value CodeGen::visit(ForStmt* stmt) {
    if (init) {
        table->pushScope();

        auto value = stmt->start->accept(this);

        auto it = new Reg();
        it->lvl = valueToL(value);
        r = it;
        push();
        table->declare(stmt->id, Value(Value::ID, offset));

        LBLabel();
        l = it;
        r = new Reg("c");
        r->lvl = l->lvl;
        mov();

        value = stmt->end->accept(this);
        l = new Reg();
        l->lvl = valueToL(value);
        r = new Reg("c");
        jmp(end(labels.top()), 
            stmt->inclusive ? GT : GE);

        stmt->block->accept(this);

        Reg* reg = new Reg("bp");
        r = new Mem(reg, bp.top() - int(*(table->lookup(stmt->id))));
        inc();
        jmp(labels.top());

        LELabel();
        r = it;
        pop();

        table->popScope();
        return Value(Value::UNIT, 0);
    }
    else {
        stmt->block->accept(this);
        return {};
    }
}

Value CodeGen::visit(WhileStmt* stmt) {
    if (init) {
        LBLabel();

        auto value = stmt->cond->accept(this);
        l = new Const(Value(Value::I64, 0));
        r = new Reg();
        cmp();

        jmp(end(labels.top()), EQ);

        stmt->block->accept(this);

        jmp(labels.top());

        LELabel();
        return Value(Value::UNIT, 0);
    }
    else {
        stmt->block->accept(this);
        return {};
    }
}

Value CodeGen::visit(PrintStmt* stmt) {
    list<string> regs = {"si", "si", "d", "c"};
    string print = "printf@PLT";
    if (init) {
        Reg* reg = new Reg("ip");
        l = new Mem(reg, stmt->strLiteral);
        r = new Reg("di");
        lea();

        auto it2 = regs.begin();
        for (auto it = stmt->args.begin(); it != stmt->args.end(); ++it, ++it2) {
            Value value = (*it)->accept(this);
            l = new Reg();
            l->lvl = valueToL(value);
            r = new Reg(*it2);
            movz();
        }

        //l = new Const(Value(Value::I64, stmt->args.size()));
        r = new Reg();
        mov();

        call(print);

        return Value (Value::UNIT, 0);
    }
    else {
        string s = LCLabel();
        out << ".string \"" << stmt->strLiteral << "\"\n"; 
        stmt->strLiteral = s;
        return {};
    }
}

Value CodeGen::visit(BreakStmt* stmt) {
    if (init) {
        Value value (Value::UNIT, 0);
        if (stmt->exp) {
            value = stmt->exp->accept(this);
        }
        jmp(end(labels.top()));
        return value;
    }
    else {
        if (stmt->exp) stmt->exp->accept(this);
        return {};
    }
}

Value CodeGen::visit(ReturnStmt* stmt) {
    if (init) {
        if (stmt->exp) {
            return stmt->exp->accept(this);
        }
        else {
            Value value = Value(Value::UNIT, 0);
            l = new Const(value);
            r = new Reg("a");
            r->lvl = l->lvl;
            mov();
            return value;
        }
    }
    else {
        stmt->exp->accept(this);
        return {};
    }
}

Value CodeGen::visit(ExpStmt* stmt) {
    if (init) {
        if (stmt->returnValue) {
            return stmt->exp->accept(this);
        }
        else {
            return Value(Value::UNIT, 0);
        }
    }
    else {
        return stmt->exp->accept(this);
    }
}

// Visit methods for functions and programs
Value CodeGen::visit(Fun* fun) {
    if (init) {
        table->pushScope();

        // l = new Const(Value(Value::I64, curOffset));
        // r = new Reg("sp");
        // sub();

        int len = {};

        for (auto param : fun->params) {
            len += typeLen(param.type);
            offset += typeLen(param.type);

            table->declare(param.id, Value(Value::ID, prevOff - len));
        }

        fun->block->accept(this);

        table->popScope();

        return Value(fun->type);
    }
    else {
        fun->block->accept(this);
        return {};
    }
}

void CodeGen::visit(Program* program) {
    table->pushScope();

    for (auto [id, fun] : program->funs) {
        Value value (Value::ID, id);
        value.fun = true;

        for (auto param : fun->params) {
            value.addType(param.type);
        }

        table->declare(id, value);

        init = false;
        out << ".section .rodata\n";
        fun->accept(this);

        // prologue
        init = true;
        out << ".text\n";
        out << ".globl " << id << '\n';
        out << ".type " << id << ", @function";
        out << id << ":\n";
        out << ".LFB" << ++lf << ":\n";
        enter();

        fun->accept(this);

        // epilogue
        out << ".LFE" << ++lf << ":\n";
        leave();
        ret();
    }

    table->popScope();
    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
}
