//
// Created by jose on 01/07/25.
//

#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "Visitor.h"

class TypeCheck final : public Visitor {
public:
    explicit TypeCheck(SymbolTable* table = nullptr) : Visitor(table) {}
    ~TypeCheck() override;
    Value visit(Block* block) override;
    Value visit(BinaryExp* exp) override;
    Value visit(UnaryExp* exp) override;
    Value visit(Literal* exp) override;
    Value visit(Variable* exp) override;
    Value visit(FunCall* exp) override;
    Value visit(IfExp* exp) override;
    Value visit(LoopExp* exp) override;
    Value visit(SubscriptExp* exp) override;
    Value visit(SliceExp* exp) override;
    Value visit(ReferenceExp* exp) override;
    Value visit(ArrayExp* exp) override;
    Value visit(UniformArrayExp* exp) override;
    Value visit(DecStmt* stmt) override;
    Value visit(AssignStmt* stmt) override;
    Value visit(CompoundAssignStmt* stmt) override;
    Value visit(ForStmt* stmt) override;
    Value visit(WhileStmt* stmt) override;
    Value visit(PrintStmt* stmt) override;
    Value visit(BreakStmt* stmt) override;
    Value visit(ReturnStmt* stmt) override;
    Value visit(ExpStmt* stmt) override;
    Value visit(Fun* fun) override;
    void visit(Program* program) override;

private:
    Value lookup(const std::string& id) const;
    void declare(const std::string& id, const Value& val) const;
    static void assertMut(const Value& val, int line, int col) ;
    static void assertType(Value::Type from, Value::Type to, int line, int col) ;
    int getScopeDepth() const;

    Value::Type currentReturnType{Value::UNDEFINED};
    int scopeDepth{};
    bool lhsContext{false};
};

#endif //TYPECHECK_H
