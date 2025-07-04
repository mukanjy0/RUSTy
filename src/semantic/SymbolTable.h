//
// Created by jose on 01/07/25.
//

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../syntactic/Exp.h"

class SymbolTable {
public:
    void pushScope();
    void popScope();
    bool declare(const std::string& name, const Value& value);
    bool update(const std::string& name, const Value& value);
    Value* lookup(const std::string& name);
    int getScopeDepth() const;

private:
    std::vector<std::unordered_map<std::string, Value>> scopes_;

};

#endif //SYMBOLTABLE_H
