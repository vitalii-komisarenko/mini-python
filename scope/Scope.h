#pragma once

#include "Instruction.h"
#include "StandardFunctions.h"
#include "variable/Variable.h"

#include <string>
#include <unordered_map>

namespace MiniPython {

class Variables {
public:
    bool has(const std::string &name);
    Variable get(const std::string &name);
    void set(const std::string &name, Variable value);
private:
    std::unordered_map<std::string, Variable> vars;
};

enum class ScopeType {
    TOP_LEVEL,
    IF,
    WHILE,
    ORDINARY_LINE,
};

class ScopeImpl;

class Scope {
public:
    Scope();

    void addChild(const Scope &child);

    void addBuiltInFunction(const std::string &name, FunctionType function);
    Variable call(const std::string &name, const InstructionParams &params);

    void setVariable(const std::string &name, Variable value);
    Variable getVariable(const std::string &name);

    Variable execute();
private:
    std::shared_ptr<ScopeImpl> scopeWithVariable(const std::string &name);

    std::shared_ptr<ScopeImpl> impl;
};

} // namespace MiniPython
