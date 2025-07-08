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
    if (value.size) {
        return typeLen(value.type) * value.size;
    }
    else if (value.ref) {
        return 8;
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
Value::Type CodeGen::LToNumericType(L lvl) {
    switch(lvl) {
        case B:  return Value::I8;
        case W:  return Value::I16;
        case D:  return Value::I32;
        case Q:  return Value::I64;
        default: return Value::I64;
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
void CodeGen::addSP(int off, bool early) {
    if (off == 0) return;

    l = new Const(Value(Value::I64, off));
    r = new Reg("sp");
    out << "add" << r->lvl << ' ' << l << ", " << r << '\n';
    if (!early) offset -= off;
}
void CodeGen::inc() {
    out << "inc" << r->lvl << ' ' << r << '\n';
}
void CodeGen::sub() {
    out << "sub" << r->lvl << ' ' << l << ", " << r << '\n';
}
void CodeGen::subSP(int off) {
    if (off == 0) return;
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
    offset += typeLen(Q);
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
void CodeGen::call(string label, bool external) {
    out << "call " << label << "\n";
    if (!external) offset += typeLen(Q);
}
void CodeGen::enter() {
    r = new Reg("bp");
    push();

    l = new Reg("sp");
    r = new Reg("bp");
    mov();

    bp.push(offset);
}
void CodeGen::leave(bool early) {
    out << "leave\n";
    if (!early) {
        offset -= typeLen(Q);
        bp.pop();
    }
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
string CodeGen::LIBLabel() {
    string label = ".LIB" + to_string(++lie);
    out << label << ":\n";
    lis.push(lie);
    return label;
}
void CodeGen::LIBLabel2() {
    string label = ".LIXB" + to_string(++lib);
    out << label << ":\n";
}
void CodeGen::LIELabel() {
    string label = ".LIE" + to_string(lis.top());
    out << label << ":\n";
    lis.pop();
}
string CodeGen::nextIf() {
    return ".LIXB" + to_string(lib + 1);
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
string CodeGen::getCurFunLbl() {
    return ".LFE" + to_string(lf);
}
string CodeGen::end(string label) {
    if (label[2] == 'F' || label[2] == 'I') {
        label[3] = 'E';
    }
    else {
        label[2] = 'E';
    }
    return label;
}
int CodeGen::getReturnDeallocate() {
    // enter (Q) will be restored with leave
    // call (Q) will be restore with ret
    return offset - 2 * typeLen(Q);
}
int CodeGen::getOffset(string label, int idx) {
    Value v = *(table->lookup(label));
    return -1 * (int(v) + idx * typeLen(typeToL(v.type)));
}

// Destructor
CodeGen::~CodeGen() = default;

// Visit methods for expressions
Value CodeGen::visit(Block* block) {
    if (init) {
        table->pushScope();

        Value val;
        for(auto stmt : block->stmts) {
            val = stmt->accept(this);
        }

        table->popScope();

        return Value(block->type);
    }
    else {
        for(auto stmt : block->stmts) {
            stmt->accept(this);
        }
        // toAllocate[curFun] = ((toAllocate[curFun] + 15) / 16) * 16;

        return {};
    }
}

Value CodeGen::visit(BinaryExp* exp) {
    if (init) {
        auto lhs = accept(exp->lhs);
        L lvl = typeToL(lhs.type);

        r = new Reg(lvl);
        push();

        auto rhs = accept(exp->rhs);
        l = new Reg(lvl);
        r = new Reg("c", lvl);
        mov();

        r = new Reg(lvl);
        pop();

        l = new Reg("c", lvl);
        r = new Reg(lvl);
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
        Value value = accept(exp->exp);
        L typeLen = typeToL(value.type);

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
        return {};
    }
}

Value CodeGen::visit(Variable* exp) {
    if (init) {
        Value value = *(table->lookup(exp->name));

        Reg* reg = new Reg("bp");
        l = new Mem(reg, getOffset(exp->name));
        r = new Reg();
        lea();

        return value;
    }
    else {
        return {};
    }
}

Value CodeGen::visit(FunCall* exp) {
    if (init) {
        auto it2 = funCallArgs.begin();

        for (auto it = exp->args.begin(); it != exp->args.end(); ++it, ++it2) {
            Value value = accept(*it);
            L lvl = typeToL(value.type);

            l = new Reg(lvl);
            r = new Reg(*it2, lvl);
            mov();
        }

        call(exp->id);

        return Value(exp->type);
    }
    else {
        for (auto arg : exp->args) {
            arg->accept(this);
        }
        return {};
    }
}

Value CodeGen::visit(IfExp* exp) {
    if (init) {
        string label = LIBLabel();

        L lvl = B;

        Value value = accept(exp->ifBranch->cond);

        l = new Const(Value(Value::BOOL, 0));
        r = new Reg(lvl);
        cmp();

        string nextLabel = end(label);
        if (!exp->elseIfBranches.empty() || exp->elseBranch) {
            nextLabel = nextIf();
        }
        jmp(nextLabel, EQ);

        exp->ifBranch->block->accept(this);

        if (nextLabel != end(label)) jmp(end(label));

        auto it = exp->elseIfBranches.begin();
        while (it != exp->elseIfBranches.end()) {
            LIBLabel2();

            IfExp::IfBranch* br = *it;
            nextLabel = end(label);
            if (++it != exp->elseIfBranches.end() || exp->elseBranch) {
                nextLabel = nextIf();
            }

            accept(br->cond);

            l = new Const(Value(Value::BOOL, 0));
            r = new Reg(lvl);
            cmp();
            jmp(nextLabel, EQ);

            br->block->accept(this);

            if (nextLabel != end(label)) jmp(end(label));
        }
        if (exp->elseBranch) {
            LIBLabel2();
            exp->elseBranch->block->accept(this);
        }

        LIELabel();
        return {exp->type};
    }
    else {
        exp->ifBranch->block->accept(this);
        for (auto branch : exp->elseIfBranches) {
            branch->block->accept(this);
        }
        if (exp->elseBranch) exp->elseBranch->block->accept(this);
        return {};
    }
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
        return {};
    }
}

Value CodeGen::visit(SubscriptExp* exp) {
    if (init) {
        Reg* reg = new Reg("bp");
        l = new Mem(reg, getOffset(exp->id));
        r = new Reg();
        lea();

        r = new Reg();
        push();

        auto value = accept(exp->exp);
        L lvl = valueToL(value);

        l = new Reg(lvl);
        r = new Reg("c", lvl);
        mov();

        l = new Const(Value(Value::I64, typeLen(value)));
        r = new Reg("c");
        mul();

        r = new Reg();
        pop();

        l = new Reg("c");
        r = new Reg();
        sub();

        value = Value(exp->type);
        value.ref = true;

        return value;
    }
    else {
        return {};
    }
}

Value CodeGen::visit(SliceExp* exp) {
    if (init) {
        auto value = Value(exp->type);
        value.ref = true;
        return value;
    }
    else {
        return {};
    }
}

Value CodeGen::visit(ReferenceExp* exp) {
    if (init) {
        auto value = exp->exp->accept(this);
        value.type = Value::I64;
        value.ref = false;
        return value;
    }
    else {
        exp->exp->accept(this);
        return {};
    }
}

Value CodeGen::visit(ArrayExp* exp) {
    if (init) {
        auto it2 = funCallArgs.begin();

        for (auto el : exp->elements) {
            auto value = accept(el);
            L lvl = typeToL(exp->type);

            l = new Reg(lvl);
            r = new Reg(*it2, lvl);
            mov();

            ++it2;
        }

        auto ret = Value(exp->type);
        ret.size = 1; // indicate it's an array
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
        auto size = accept(exp->size);
        r = new Reg();
        push();

        auto value = accept(exp->value);
        L lvl = valueToL(value);

        r = new Reg("c");
        pop();

        l = new Const(Value(Value::I64, 0));
        r = new Reg("r11");
        mov();

        LBLabel();

        l = new Reg("c");
        r = new Reg("r11");
        cmp();

        jmp(end(labels.top()), GE);

        r = new Reg(lvl);
        push();

        r = new Reg("r11");
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

        L lvl = typeToL(value.type);
        allocated[curFun] += typeLen(lvl);

        Value val = Value(value.type, allocated[curFun]);
        val.ref = true;
        val.size = value.size;
        table->declare(stmt->id, val);

        Reg* reg = new Reg("bp");

        if (stmt->rhs) {
            auto rhs = accept(stmt->rhs);

            if (value.size) {
                auto it2 = funCallArgs.begin();

                for (int i=0; i<value.size; ++i, ++it2) {
                    l = new Reg(*it2, lvl);
                    r = new Mem(reg, getOffset(stmt->id, i), lvl);
                    mov();
                }

                allocated[curFun] += typeLen(value) - typeLen(lvl);
            }
            else {
                l = new Reg(lvl);
                r = new Mem(reg, getOffset(stmt->id), lvl);
                mov();
            }
        }
        return Value(Value::UNIT, 0);
    }
    else {
        toAllocate[curFun] += typeLen(stmt->var);
        if (stmt->var.type == Value::STR) {
            stmt->rhs->accept(this);
        }
        return {};
    }
}

Value CodeGen::visit(AssignStmt* stmt) {
    if (init) {
        auto lhs = stmt->lhs->accept(this);

        L lvl = typeToL(lhs.type);

        if (lhs.size == 0) {
            r = new Reg();
            push();

            auto rhs = accept(stmt->rhs);

            r = new Reg("c");
            pop();

            l = new Reg(valueToL(rhs));
            auto reg = new Reg("c");
            r = new Mem(reg, 0);
            mov();
        }
        else {
            auto it2 = funCallArgs.begin();

            Value value;

            string id;

            // Lhs is either Variable
            auto var = dynamic_cast<Variable*>(stmt->lhs);
            if (var) {
                id = var->name;
            }
            // or SubscriptExp
            auto var2 = dynamic_cast<SubscriptExp*>(stmt->lhs);
            if (var2) {
                id = var2->id;
            }

            value = *(table->lookup(id));

            auto reg = new Reg("bp");
            for (int i=0; i<value.size; ++i, ++it2) {
                l = new Reg(*it2, lvl);
                r = new Mem(reg, getOffset(id, i), lvl);
                mov();
            }
        }
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
        L lvl = typeToL(lhs.type);

        // store temporarily address of lhs
        l = new Reg(ptrLen);
        r = new Reg("b", ptrLen);
        mov();

        // push value in address
        Reg* reg = new Reg(ptrLen);
        l = new Mem(reg, 0);
        r = new Reg(lvl);
        mov();
        push();

        auto rhs = accept(stmt->rhs);

        l = new Reg(lvl);
        r = new Reg("c", lvl);
        mov();

        r = new Reg(lvl);
        pop();

        l = new Reg("c", lvl);
        r = new Reg(lvl);

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
        l = new Reg(lvl);
        reg = new Reg("b");
        r = new Mem(reg, 0, ptrLen);
        mov();

        return Value(Value::UNIT);
    }
    else {
        return {};
    }
}

Value CodeGen::visit(ForStmt* stmt) {
    if (init) {
        table->pushScope();

        auto value = accept(stmt->start);
        L lvl = valueToL(value);

        allocated[curFun] += typeLen(lvl);
        table->declare(stmt->id, Value(value.type, allocated[curFun], true));

        l = new Reg(lvl);
        Reg* reg = new Reg("bp");
        auto it = new Mem(reg, getOffset(stmt->id), lvl);
        r = it;
        mov();

        LBLabel();
        value = accept(stmt->end);
        l = new Reg(valueToL(value));
        r = it;
        cmp();

        jmp(end(labels.top()), stmt->inclusive ? GT : GE);

        stmt->block->accept(this);

        r = it;
        inc();
        jmp(labels.top());

        LELabel();

        table->popScope();

        return Value(Value::UNIT, 0);
    }
    else {
        toAllocate[curFun] += typeLen(stmt->start->type);
        stmt->block->accept(this);
        return {};
    }
}

Value CodeGen::visit(WhileStmt* stmt) {
    if (init) {
        LBLabel();

        auto value = accept(stmt->cond);
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
    string print = "printf@PLT";
    if (init) {
        auto it2 = funCallArgs.begin();

        Reg* reg = new Reg("ip");
        l = new Mem(reg, stmt->strLiteral);
        r = new Reg(*it2);
        lea();

        for (auto it = stmt->args.begin(); it != stmt->args.end(); ++it) {
            ++it2;

            Value value = accept(*it);

            L lvl = typeToL(value.type);

            Reg* valReg = new Reg(lvl);

            if (value.type == Value::BOOL) {
                // bool value in valReg -> convert to pointer to "true"/"false"
                Reg* regTrue = new Reg("r11");
                reg = new Reg("ip");
                l = new Mem(reg, boolTrueLabel);
                r = regTrue;
                lea();

                Reg* regFalse = new Reg("r12");
                reg = new Reg("ip");
                l = new Mem(reg, boolFalseLabel);
                r = regFalse;
                lea();

                l = new Const(Value(Value::BOOL, 0));
                r = valReg;
                cmp();

                l = regFalse;
                r = regTrue;
                cmov(EQ);

                l = regTrue;
                r = new Reg(*it2, Q);
                mov();
            } else {
                l = valReg;
                r = new Reg(*it2, lvl);
                mov();
            }
        }

        l = new Const(Value(Value::I64, 0));
        r = new Reg();
        mov();

        call(print, true);

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
        Value value;
        if (stmt->exp) {
            value = accept(stmt->exp);
        }
        else {
            value = Value(Value::UNIT, 0);
            l = new Const(value);
            r = new Reg("a", l->lvl);
            mov();
        }

        jmp(getCurFunLbl());

        return value;
    }
    else {
        stmt->exp->accept(this);
        return {};
    }
}

Value CodeGen::visit(ExpStmt* stmt) {
    if (init) {
        return stmt->exp->accept(this);
    }
    else {
        return stmt->exp->accept(this);
    }
}

// Visit methods for functions and programs
Value CodeGen::visit(Fun* fun) {
    if (init) {
        table->pushScope();

        subSP(toAllocate[curFun]);

        // assigning arguments to parameters
        auto it2 = funCallArgs.begin();
        Reg* reg = new Reg("bp");

        for (auto param : fun->params) {
            L lvl = typeToL(param.type);

            allocated[curFun] += typeLen(lvl);

            Value value = Value(param.type, allocated[curFun]);
            value.ref = true;
            table->declare(param.id, value);

            l = new Reg(*it2, lvl);
            r = new Mem(reg, getOffset(param.id), lvl);
            mov();

            ++it2;
        }

        fun->block->accept(this);

        addSP(toAllocate[curFun]);

        table->popScope();

        return Value(fun->type);
    }
    else {
        for (auto param : fun->params) {
            L lvl = typeToL(param.type);
            toAllocate[curFun] += typeLen(lvl);
        }
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

        curFun = id;

        out << ".section .rodata\n";
        init = false;
        fun->accept(this);

        // prologue
        init = true;
        out << ".text\n";
        out << ".globl " << id << '\n';
        out << ".type " << id << ", @function\n";
        out << id << ":\n";

        // prologue
        LFBLabel();
        enter();

        fun->accept(this);

        // epilogue
        LFELabel();
        leave();
        ret();
    }

    table->popScope();
    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
}

Value CodeGen::accept(Block* block) {
    Value value = block->accept(this);

    if (value.ref) {
        Reg* reg = new Reg();
        l = new Mem(reg, 0);
        r = new Reg(typeToL(value.type));
        mov();
    }

    value.ref = false;
    return value;
}
Value CodeGen::accept(Exp* exp) {
    Value value = exp->accept(this);

    if (value.ref) {
        Reg* reg = new Reg();
        l = new Mem(reg, 0);
        r = new Reg(typeToL(value.type));
        mov();
        value.ref = false;
    }

    return value;
}
Value CodeGen::accept(Stmt* stmt) {
    Value value = stmt->accept(this);

    if (value.ref) {
        Reg* reg = new Reg();
        l = new Mem(reg, 0);
        r = new Reg(typeToL(value.type));
        mov();
        value.ref = false;
    }

    return value;
}
Value CodeGen::accept(Fun* fun) {
    Value value = fun->accept(this);

    if (value.ref) {
        Reg* reg = new Reg();
        l = new Mem(reg, 0);
        r = new Reg(typeToL(value.type));
        mov();
        value.ref = false;
    }

    return value;
}
