#ifndef FUN_H
#define FUN_H

#define FRIENDS friend class CodeGen; friend class TypeCheck; friend class NameRes;

#include "Stmt.h"

struct Param {
    int line;
    int col;
    Value::Type type;
    std::string id;
    Param() = default;
    Param(int line, int col, Value::Type type, std::string id) 
        : line(line), col(col), type(type), id(id) {}
};

class Fun {
    FRIENDS
    int line;
    int col;
    Value::Type type;
    std::list<Param> params;
    Block* block;

public:
    Fun(int line, int col, Value::Type type, std::list<Param> params, Block *block)
            : line(line), col(col), type(type), params(std::move(params)), block(block) {}
    ~Fun();
    Value accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, const Fun* fun);
};

class Program {
    FRIENDS
    std::list<std::pair<std::string,Fun*>> funs;

public:
    explicit Program(std::list<std::pair<std::string,Fun*>>  funs) : funs(std::move(funs)) {}
    ~Program();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, Program* program);
};

#endif
