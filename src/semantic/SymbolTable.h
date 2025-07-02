//
// Created by jose on 01/07/25.
//

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../syntax/Exp.h"

class SymbolTable {
public:
    void pushScope();
    void popScope();
    bool declare(const std::string& name, const Var& value);
    bool update(const std::string& name, const Var& value);
    Var* lookup(const std::string& name);

private:
    std::vector<std::unordered_map<std::string, Var>> scopes_;

};

#endif //SYMBOLTABLE_H
