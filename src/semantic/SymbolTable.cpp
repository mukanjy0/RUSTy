//
// Created by jose on 01/07/25.
//

#include "SymbolTable.h"

void SymbolTable::pushScope() {
    scopes_.emplace_back();
}

void SymbolTable::popScope() {
    if (!scopes_.empty()) {
        scopes_.pop_back();
    }
}

bool SymbolTable::declare(const std::string& name, const Var& value) {
    if (scopes_.empty()) {
        pushScope();
    }
    auto& current = scopes_.back();
    auto [_, inserted] = current.emplace(name, value);
    return inserted;
}

bool SymbolTable::update(const std::string& name, const Var& value) {
    if (auto* sym = lookup(name)) {
        *sym = value;
        return true;
    }
    return false;
}

Var* SymbolTable::lookup(const std::string& name) {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {  // reverse iteration
        auto found = it->find(name);
        if (found != it->end()) {
            return &found->second;
        }
    }
    return nullptr;
}
