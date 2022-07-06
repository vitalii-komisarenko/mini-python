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

    using Func = Variable(const InstructionParams&);
    void addBuiltInFunction(const std::string &name, Func function);
    Variable call(const std::string &name, const InstructionParams &params);

    Variable execute();
private:
    std::shared_ptr<ScopeImpl> impl;
};

} // namespace MiniPython
