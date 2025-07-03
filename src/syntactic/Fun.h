#ifndef FUN_H
#define FUN_H

#define FRIENDS friend class CodeGen; friend class TypeCheck; friend class NameRes;

#include <utility>

#include "Stmt.h"

struct Param {
    Value::Type type{};
    std::string id;
    Param() = default;
    Param(Value::Type type, std::string id) : type(type), id(std::move(id)) {}
};

class Fun {
    FRIENDS
    std::list<Param> params;
    Block* block;

public:
    explicit Fun(std::list<Param> params, Block *block)
            : params(std::move(params)), block(block) {}
    ~Fun();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, const Fun* fun);
};

class Program {
    FRIENDS
    std::map<std::string,Fun*> funs;

public:
    explicit Program(std::map<std::string,Fun*> funs) : funs(std::move(funs)) {}
    ~Program();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, Program* program);
};

#endif
