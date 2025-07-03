#include "Fun.h"

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

Program::~Program() {
    for (auto [id, fun] : funs) {
        delete fun;
    }
    funs.clear();
}
std::ostream& operator<<(std::ostream& out, Program* program) {
    for (auto [id, fun] : program->funs) {
        out << "fn " << id << fun << "\n";
    }
    return out;
}
