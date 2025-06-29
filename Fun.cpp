#include "Fun.h"
#include <iostream>

Fun::~Fun() {
    delete block;
}
std::ostream& operator<<(std::ostream& out, Fun* fun) {
    out << "(";
    if (fun->params.empty()) {
        size_t i {};
        for (auto param : fun->params) {
            out << param.type << ' ' << param.id;
            if (++i < fun->params.size()) out << ", ";
        }
    }
    out << ")\n";
    out << fun->block;
    return out;
}

FunList::~FunList() {
    for (auto [id, fun] : funs) {
        delete fun;
    }
    funs.clear();
}
std::ostream& operator<<(std::ostream& out, FunList* funList) {
    for (auto [id, fun] : funList->funs) {
        out << "fn " << id << fun << "\n";
    }
    return out;
}

Program::~Program() {
    delete funList;
}
std::ostream& operator<<(std::ostream& out, Program* program) {
    out << program->funList;
    return out;
}
