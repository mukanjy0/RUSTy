//
// Created by jose on 01/07/25.
//

#include "SymbolTable.h"

#include <ranges>

void SymbolTable::pushScope() {
    scopes_.emplace_back();
}

void SymbolTable::popScope() {
    if (!scopes_.empty()) {
        scopes_.pop_back();
    }
}

bool SymbolTable::declare(const std::string& name, const Value& value) {
    if (scopes_.empty()) {
        pushScope();
    }
    auto& current = scopes_.back();
    auto [_, inserted] = current.emplace(name, value);
    return inserted;
}

bool SymbolTable::update(const std::string& name, const Value& value) {
    if (auto* sym = lookup(name)) {
        *sym = value;
        return true;
    }
    return false;
}

Value* SymbolTable::lookup(const std::string& name) {
    for (auto & scope : std::ranges::reverse_view(scopes_)) {  // reverse iteration
        auto found = scope.find(name);
        if (found != scope.end()) {
            return &found->second;
        }
    }
    return nullptr;
}
