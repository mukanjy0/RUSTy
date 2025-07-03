#include "Fun.h"

Fun::~Fun() {
    delete block;
}
std::ostream& operator<<(std::ostream& out, const Fun* fun) {
    out << "(";
    if (fun->params.empty()) {
        size_t i {};
        for (const auto& param : fun->params) {
            out << param.type << ' ' << param.id;
            if (++i < fun->params.size()) out << ", ";
        }
    }
    out << ")";
    if (fun->type != Value::UNDEFINED) out << " -> " << fun->type;
    out << "\n";
    out << fun->block;
    return out;
}

Program::~Program() {
    for (const auto& [id, fun]: funs) {
        delete fun;
    }
    funs.clear();
}
std::ostream& operator<<(std::ostream& out, Program* program) {
    for (const auto& [id, fun] : program->funs) {
        out << "fn " << id << fun << "\n";
    }
    return out;
}
