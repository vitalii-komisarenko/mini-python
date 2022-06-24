#pragma once

#include "Instruction.h"
#include "variable/Variable.h"

#include <unordered_map>
#include <vector>

namespace MiniPython {

class Variables {
public:
    bool has(const std::string &name);
    Variable get(const std::string &name);
private:
    std::unordered_map<std::string, Variable> vars;
};

enum class ScopeType {
    TOP_LEVEL,
    IF,
    WHILE,
};

class _Scope;
using Scope = std::shared_ptr<_Scope>;

class _Scope {
public:
    void addChild(Scope child);
    bool isTopLevelScope();
    ScopeType type;

    Variable execute();
    Instruction instruction;
private:
    std::vector<Scope> children;
};

} // namespace MiniPython
