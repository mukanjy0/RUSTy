#include "Stmt.h"

struct Param {
    Var::Type type;
    std::string id;
    Param(Var::Type type, std::string id) : type(type), id(id) {}
};

class Fun {
    std::list<Param> params;
    Block* block;

public:
    explicit Fun(Block *block) : block(block) {}
    ~Fun();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, Fun* fun);
};

class FunList {
    std::map<std::string,Fun*> funs;

public:
    FunList() = default;
    explicit FunList(std::map<std::string,Fun*> funs) : funs(std::move(funs)) {}
    ~FunList();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, FunList* funList);
};

class Program {
    FunList* funList {};

public:
    explicit Program(FunList *funList) : funList(funList) {}
    ~Program();
    void accept(Visitor* visitor);
    friend std::ostream& operator<<(std::ostream& out, Program* program);
};
