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
class LineTree;

class Scope {
public:
    Scope();
    Scope(const LineTree &lineTree);

    void addChild(const Scope &child);

    Variable call(const std::string &name, const InstructionParams &params);

    void setVariable(const std::string &name, Variable value);
    Variable getVariable(const std::string &name);

    Variable execute();

    std::shared_ptr<ScopeImpl> impl;
private:
    Scope(const LineTree &lineTree, bool isTopLevel);

    std::shared_ptr<ScopeImpl> scopeWithVariable(const std::string &name);
};

class ScopeImpl {
public:
    ScopeImpl()
        : type(ScopeType::TOP_LEVEL) {}

    ScopeType type;

    Instruction instruction;
    Variables vars;
    std::weak_ptr<ScopeImpl> parent;
    std::vector<Scope> children;

    friend class Scope;
private:
    bool isTopLevelScope();
};

} // namespace MiniPython
