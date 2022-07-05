#pragma once

#include "Instruction.h"
#include "StandardFunctions.h"
#include "variable/Variable.h"

#include <string>
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
    ORDINARY_LINE,
};

class _Scope;
using Scope = std::shared_ptr<_Scope>;

class _Scope {
public:
    void addChild(Scope child);
    ScopeType type;

    Variable execute();
    Instruction instruction;
    Variable call(const std::string &name, const InstructionParams &params);
//private:
    Variables vars;
    std::weak_ptr<_Scope> parent;
    std::vector<Scope> children;
    using Func = Variable(const InstructionParams&);
    std::unordered_map<std::string, Func*> builtInFunctions;
private:
    bool isTopLevelScope();
};

} // namespace MiniPython
