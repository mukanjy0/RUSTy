#include "CodeGen.h"

Operand::~Operand() = default;
std::ostream& operator<<(std::ostream& out, Operand* op) {
    op->print(out);
    return out;
}

Reg::~Reg() {}
void Reg::print(ostream& out) {
    out << "%";
    if (reg.length() > 1 && isdigit(reg[1])) {
        out << reg;
        switch(lvl) {
            case B: out << 'b'; break;
            case W: out << 'w'; break;
            case D: out << 'd'; break;
            default: break;
        }
        return;
    }
    if (lvl == D) out << 'e';
    else if (lvl == Q) out << 'r';
    out << reg;
    if (lvl != B) {
        if (reg.length() == 1) out << "x";
    }
    else out << 'l';
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
        case B: out << 'b'; break;
        case W: out << 'w'; break;
        case D: out << 'l'; break;
        case Q: out << 'q'; break;
    }
    return out;
}

char CodeGen::nextL(L lvl) {
    switch (lvl) {
        case B: return 'w';
        case W: return 'l';
        case D: return 'q';
        default: return 'o';
    }
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
        case Value::I8:
        case Value::CHAR: return 1;
        case Value::I16: return 2;
        case Value::I32: return 4;
        default: return 8;
    }
}


int CodeGen::typeLen(Value value) {
    if (value.ref || value.size) return 8;
    if (value.size) {
        return typeLen(value.type) * value.size + 8;
    }
    else if (value.right) {
        return 2 * 8;
    }
    else {
        return typeLen(value.type);
    }
}

L CodeGen::typeToL(Value::Type type) {
    switch(type) {
        case Value::CHAR: 
        case Value::I8: 
        case Value::BOOL: return B;
        case Value::I16: return W;
        case Value::I32:
        case Value::UNIT: return D;
        case Value::I64:
        case Value::ID:
        case Value::STR: // it will always be a pointer
        default: return Q;
    }
}

L CodeGen::valueToL(Value value) {
    if (value.ref) return Q;

    return typeToL(value.type);
}

void CodeGen::mov() {
    auto c = dynamic_cast<Const*>(l);
    if (c) {
        out << "mov" << r->lvl << ' ' << l << ", " << r << '\n';
        return;
    }
    auto m = dynamic_cast<Mem*>(r);
    if (m) {
        out << "mov" << l->lvl << ' ' << l << ", " << r << '\n';
        return;
    }

    if (l->lvl < r->lvl) {
        return movs(); // sign-extend
    }
    else if (l->lvl > r->lvl) {
        l->lvl = r->lvl; // downgrade to destination's level
    }
    out << "mov" << (m ? l->lvl : r->lvl) << ' ' << l << ", " << r << '\n';
}
void CodeGen::movz() {
    out << "movz" << l->lvl << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::movs() {
    out << "movs" << l->lvl << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::add() {
    out << "add" << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::addSP(int off) {
    l = new Const(Value(Value::I64, off));
    r = new Reg("sp");
    out << "add" << r->lvl << ' ' << l << ", " << r << '\n';
    offset -= off;
}
void CodeGen::inc() {
    out << "inc" << r->lvl << ' ' << r << '\n';
}
void CodeGen::sub() {
    out << "sub" << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::subSP(int off) {
    l = new Const(Value(Value::I64, off));
    r = new Reg("sp");
    out << "sub" << r->lvl << ' ' << l << ", " << r << '\n';
    offset += off;
}
void CodeGen::dec() {
    out << "dec" << r->lvl << ' ' << r << '\n';
}
void CodeGen::mul() {
    out << "imul" << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::div() {
    switch (r->lvl) {
        case Q:
            out << "cqto\n";
            break;
        case D:
            out << "cdq\n";
            break;
        case W:
            out << "cwd\n";
            break;
        case B:
            out << "cbtw\n";
            break;
        default:
            break;
    }
    out << "idiv" << ' ' << l << '\n';
}
void CodeGen::land() {
    out << "and" << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::lor() {
    out << "or" << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::bnot() {
    out << "not" << r->lvl << ' ' << r << '\n';
}
void CodeGen::push() {
    if (typeLen(r->lvl) < typeLen(Q)) {
        l = r;
        r = new Reg("r11");
        mov();
    }
    out << "push" << r->lvl << ' ' << r << '\n';
    offset += typeLen(r->lvl);
}
void CodeGen::pop() {
    if (typeLen(r->lvl) < typeLen(Q)) {
        l = r;
        r = new Reg("r11");
        out << "pop" << r->lvl << ' ' << r << '\n';
        r = l;
        l = new Reg("r11");
        mov();
    }
    else {
        out << "pop" << r->lvl << ' ' << r << '\n';
    }
    offset -= typeLen(Q);
}
void CodeGen::lea() {
    out << "lea" << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::cmp() {
    out << "cmp" << r->lvl << ' ' << l << ", " << r << '\n';
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
    out << "cmov" << cond << ' ' << l << ", " << r << '\n';
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
    out << label << ":\n";
    return label;
}
void CodeGen::LBLabel() {
    string label = ".LB" + to_string(++lb);
    out << label << ":\n";
    lbs.push(lb);
    labels.push(label);
}
void CodeGen::LELabel() {
    string label = ".LE" + to_string(lbs.top());
    out << label << ":\n";
    lbs.pop();
    labels.pop();
}
void CodeGen::LILabel() {
    string label = ".LI" + to_string(++li);
    out << label << ":\n";
    lis.push(li);
}
void CodeGen::LIELabel() {
    string label = ".LIE" + to_string(lis.top());
    out << label << ":\n";
    lis.pop();
}
string CodeGen::endI(string label) {
    return ".LIE" + label.substr(3);
}
void CodeGen::LFBLabel() {
    string label = ".LFB" + to_string(++lf);
    out << label << ":\n";
    labels.push(label);
}
void CodeGen::LFELabel() {
    string label = ".LFE" + to_string(lf);
    out << label << ":\n";
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
int CodeGen::getOffset(string label) {
    return bp.top() - int(*(table->lookup(label)));
}

// Destructor
CodeGen::~CodeGen() = default;

// Visit methods for expressions
Value CodeGen::visit(Block* block) {
    if (init) {
        table->pushScope();

        enter();

        subSP(toAllocate[block]);

        Value val;
        cur.push(block);
        for(auto stmt : block->stmts) {
            val = stmt->accept(this);
        }
        cur.pop();

        addSP(toAllocate[block]);

        leave();

        table->popScope();
    }
    else {
        cur.push(block);
        for(auto stmt : block->stmts) {
            stmt->accept(this);
        }
        toAllocate[cur.top()] = ((toAllocate[block] + 15) / 16) * 16;
        cur.pop();
    }
    return {};
}

Value CodeGen::visit(BinaryExp* exp) {
    if (init) {
        auto lhs = exp->lhs->accept(this);

        L typeLen = typeToL(lhs.type);

        if (lhs.ref) {
            Reg* reg = new Reg();
            l = new Mem(reg, 0);
            r = new Reg(typeLen);
            mov();
        }
        push();

        auto rhs = exp->rhs->accept(this);

        if (rhs.ref) {
            Reg* reg = new Reg();
            l = new Mem(reg, 0);
            r = new Reg(typeLen);
            mov();
        }

        l = new Reg(typeLen);
        r = new Reg("c", typeLen);
        mov();

        r = new Reg(typeLen);
        pop();

        l = new Reg("c", typeLen);
        r = new Reg(typeLen);
        switch (exp->op) {
            case BinaryExp::LAND:
                land();
                return Value(Value::BOOL);
            case BinaryExp::LOR:
                lor();
                return Value(Value::BOOL);
            case BinaryExp::GT:
                cmp();
                r = new Reg(B);
                set(GT);
                return Value(Value::BOOL);
            case BinaryExp::LT:
                cmp();
                r = new Reg(B);
                set(LT);
                return Value(Value::BOOL);
            case BinaryExp::GE:
                cmp();
                r = new Reg(B);
                set(GE);
                return Value(Value::BOOL);
            case BinaryExp::LE:
                cmp();
                r = new Reg(B);
                set(LE);
                return Value(Value::BOOL);
            case BinaryExp::EQ:
                cmp();
                r = new Reg(B);
                set(EQ);
                return Value(Value::BOOL);
            case BinaryExp::NEQ:
                cmp();
                r = new Reg(B);
                set(NE);
                return Value(Value::BOOL);
            case BinaryExp::PLUS:
                add();
                return Value(lhs.type);
            case BinaryExp::MINUS:
                sub();
                return Value(lhs.type);
            case BinaryExp::TIMES:
                mul();
                return Value(lhs.type);
            case BinaryExp::DIV:
                div();
                return Value(lhs.type);
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
        Value value = exp->exp->accept(this);

        L typeLen = typeToL(value.type);

        if (value.ref) {
            Reg* reg = new Reg();
            l = new Mem(reg, 0);
            r = new Reg(typeLen);
            mov();
        }

        switch (exp->op) {
            case UnaryExp::LNOT:
                l = new Const(Value(Value::BOOL, 0));
                r = new Reg(B);
                cmp();
                set(EQ);
                break;
            default:
                throw std::runtime_error("Invalid unary operation");
        }

        return Value(Value::BOOL);
    }
    else {
        exp->exp->accept(this);
        return {};
    }
}

Value CodeGen::visit(Literal* exp) {
    if (init) {
        L lvl = valueToL(exp->value);
        l = new Const(exp->value, lvl);
        r = new Reg(lvl);
        mov();

        return exp->value;
    }
    else {
        if (exp->value.type == Value::STR) {
            string label = ".LC" + to_string(++lc);
            out << label << '\n';
            out << ".string \"" << exp->value.stringValues.front() << "\"\n"; 

            exp->value.type = Value::STR;
            exp->value.ref = true;
            exp->value.stringValues.front() = label;
        }
    }
    return {};
}

Value CodeGen::visit(Variable* exp) {
    if (init) {
        Value value = *(table->lookup(exp->name));

        Reg* reg = new Reg("bp");
        l = new Mem(reg, bp.top() - int(value));
        r = new Reg();
        lea();

        return value;
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
        int id = ++li;
        string endLabel = ".LIE" + to_string(id);

        exp->ifBranch->cond->accept(this);
        l = new Const(Value(Value::I64, 0));
        r = new Reg();
        cmp();

        string nextLabel = endLabel;
        if (!exp->elseIfBranches.empty() || exp->elseBranch) {
            nextLabel = ".LI" + to_string(++li);
        }
        jmp(nextLabel, EQ);

        exp->ifBranch->block->accept(this);
        jmp(endLabel);

        if (!exp->elseIfBranches.empty() || exp->elseBranch) {
            out << nextLabel << ":\n";
            auto it = exp->elseIfBranches.begin();
            while (it != exp->elseIfBranches.end()) {
                IfExp::IfBranch* br = *it;
                ++it;
                string afterLabel = (it != exp->elseIfBranches.end() || exp->elseBranch)
                        ? ".LI" + to_string(++li)
                        : endLabel;

                br->cond->accept(this);
                l = new Const(Value(Value::I64, 0));
                r = new Reg();
                cmp();
                jmp(afterLabel, EQ);

                br->block->accept(this);
                jmp(endLabel);

                out << afterLabel << ":\n";
            }

            if (exp->elseBranch) {
                exp->elseBranch->block->accept(this);
            }
        }

        out << endLabel << ":\n";
        return {exp->type};
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
        LBLabel();
        exp->block->accept(this);
        jmp(labels.top());
        LELabel();
        return {exp->type};
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
        auto value = exp->exp->accept(this);
        value.ref = true;
        return value;
    }
    else {
        exp->exp->accept(this);
        return {};
    }
}

Value CodeGen::visit(ArrayExp* exp) {
    if (init) {
        r = new Reg();
        r->lvl = typeToL(exp->type);

        for (auto el : exp->elements) {
            auto value = el->accept(this);
            push();
        }
        auto ret = Value(exp->type);
        ret.size = 1;
        return ret;
    }
    else {
        for (auto el : exp->elements) {
            el->accept(this);
        }
        return {};
    }
}

Value CodeGen::visit(UniformArrayExp* exp) {
    if (init) {
        auto size = exp->size->accept(this);
        r = new Reg();
        push();

        auto value = exp->value->accept(this);
        auto val = new Reg();
        val->lvl = valueToL(value);

        r = new Reg("c");
        pop();

        l = new Const(Value(Value::I64, 0));
        r = new Reg("d");
        mov();

        LBLabel();

        l = new Reg("c");
        r = new Reg("d");
        cmp();

        jmp(end(labels.top()), LT);

        r = val;
        push();

        r = new Reg("d");
        inc();
        jmp(labels.top());

        LELabel();

        auto ret = Value(value.type);
        ret.size = 1;
        return ret;
    }
    else {
        exp->value->accept(this);
        return {};
    }
}

// Visit methods for statements
Value CodeGen::visit(DecStmt* stmt) {
    if (init) {
        auto value = stmt->var;
        Block* b = cur.top();

        allocated[b] += typeLen(value);
        Value val = Value(value.type, bp.top() + allocated[b]);
        val.ref = true;
        table->declare(stmt->id, val);

        if (stmt->rhs) {
            auto rhs = stmt->rhs->accept(this);
            Reg* reg;

            L len = typeToL(value.type);

            if (value.size) {
                for (int i=0; i<value.size; ++i) {
                    allocated[b] += typeLen(value.type);

                    r = new Reg(len);
                    pop();

                    l = new Reg(len);
                    reg = new Reg("bp");
                    r = new Mem(reg, -allocated[b]); r->lvl = len;
                    mov();
                }
            }
            else {
                l = new Reg(len);

                reg = new Reg("bp");
                r = new Mem(reg, -allocated[b], len);
                mov();
            }
        }
        return Value(Value::UNIT, 0);
    }
    else {
        toAllocate[cur.top()] += typeLen(stmt->var);
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

        l = new Reg(valueToL(rhs));
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
        auto lhs = stmt->lhs->accept(this);

        L ptrLen = valueToL(lhs);
        L typeLen = typeToL(lhs.type);

        // store temporarily address of lhs
        l = new Reg(ptrLen);
        r = new Reg("b", ptrLen);
        mov();

        if (lhs.ref) {
            Reg* reg = new Reg(ptrLen);
            l = new Mem(reg, 0);
            r = new Reg(typeLen);
            mov();
        }
        push();

        auto rhs = stmt->rhs->accept(this);

        if (rhs.ref) {
            Reg* reg = new Reg(ptrLen);
            l = new Mem(reg, 0);
            r = new Reg(typeLen);
            mov();
        }

        l = new Reg(typeLen);
        r = new Reg("c", typeLen);
        mov();

        r = new Reg(typeLen);
        pop();

        l = new Reg("c", typeLen);
        r = new Reg(typeLen);

        switch (stmt->op) {
            case BinaryExp::PLUS:
                add();
                break;
            case BinaryExp::MINUS:
                sub();
                break;
            case BinaryExp::TIMES:
                mul();
                break;
            case BinaryExp::DIV:
                div();
                break;
            default:
                throw std::runtime_error("Invalid binary operation");
        }

        // store result of operation in previously cached address
        l = new Reg(typeLen);
        Reg* reg = new Reg("b");
        r = new Mem(reg, 0, ptrLen);
        mov();
    }
    return {};
}

Value CodeGen::visit(ForStmt* stmt) {
    if (init) {
        table->pushScope();

        auto value = stmt->start->accept(this);

        r = new Reg();
        r->lvl = valueToL(value);
        push();
        table->declare(stmt->id, Value(Value::ID, offset));
        int off = getOffset(stmt->id);

        Reg* reg = new Reg("bp");
        auto it = new Mem(reg, off);

        LBLabel();
        value = stmt->end->accept(this);

        l = new Reg(valueToL(value));
        r = it;
        cmp();

        jmp(end(labels.top()), 
            stmt->inclusive ? LE : LT);

        stmt->block->accept(this);

        r = it;
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
    list<string> regs = {"si", "d", "c"};
    string print = "printf@PLT";
    if (init) {
        Reg* reg = new Reg("ip");
        l = new Mem(reg, stmt->strLiteral);
        r = new Reg("di");
        lea();

        auto it2 = regs.begin();
        for (auto it = stmt->args.begin(); it != stmt->args.end(); ++it, ++it2) {
            Value value = (*it)->accept(this);

            L typeLen = typeToL(value.type);

            if (value.ref) {
                reg = new Reg();
                l = new Mem(reg, 0);
            }
            else {
                l = new Reg(typeLen);
            }
            Reg* valReg = new Reg(typeLen);
            r = valReg;
            mov();

            if (value.type == Value::BOOL) {
                // bool value in valReg -> convert to pointer to "true"/"false"
                Reg* regTrue = new Reg("r10");
                reg = new Reg("ip");
                l = new Mem(reg, boolTrueLabel);
                r = regTrue;
                lea();

                Reg* regFalse = new Reg("r11");
                reg = new Reg("ip");
                l = new Mem(reg, boolFalseLabel);
                r = regFalse;
                lea();

                l = new Const(Value(Value::I64, 0));
                r = valReg;
                cmp();

                l = regFalse;
                r = regTrue;
                cmov(EQ);

                l = regTrue;
                r = new Reg(*it2);
                mov();
            } else {
                l = valReg;
                r = new Reg(*it2);
                mov();
            }
        }

        int argCnt = stmt->args.size();
        l = new Const(Value(Value::I64, argCnt));
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
            r = new Reg("a", l->lvl);
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

        int len = {};

        for (auto param : fun->params) {
            len += typeLen(param.type);
            offset += typeLen(param.type);

            Value value = Value(param.type, prevOff - len);
            value.ref = true;
            table->declare(param.id, value);
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

    // emit boolean string constants once
    out << ".section .rodata\n";
    if (boolTrueLabel.empty()) {
        boolTrueLabel = LCLabel();
        out << ".string \"true\"\n";
        boolFalseLabel = LCLabel();
        out << ".string \"false\"\n";
    }

    for (auto [id, fun] : program->funs) {
        Value value (fun->type, id);
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
        out << ".type " << id << ", @function\n";
        out << id << ":\n";
        LFBLabel();

        fun->accept(this);

        // epilogue
        LFELabel();
        ret();
    }

    table->popScope();
    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
}
