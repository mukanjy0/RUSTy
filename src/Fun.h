#ifndef FUN_H
#define FUN_H

#include "Stmt.h"

struct Param {
    Var::Type type;
    std::string id;
    Param() = default;
    Param(Var::Type type, std::string id) : type(type), id(id) {}
};

class Fun {
    std::list<Param> params;
    Block* block;

public:
    explicit Fun(std::list<Param> params, Block *block) 
            : params(params), block(block) {}
    ~Fun();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, Fun* fun);
};

class Program {
    std::list<std::pair<std::string,Fun*>> funs;

public:
    explicit Program(std::list<std::pair<std::string,Fun*>>  funs) : funs(std::move(funs)) {}
    ~Program();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, Program* program);
};

#endif
