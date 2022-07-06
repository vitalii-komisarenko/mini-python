#include "Scope.h"

#include <stdexcept>

namespace MiniPython {

class ScopeImpl {
public:
    ScopeImpl()
        : type(ScopeType::TOP_LEVEL) {}

    ScopeType type;

    Instruction instruction;
    Variables vars;
    std::weak_ptr<ScopeImpl> parent;
    std::vector<Scope> children;
    std::unordered_map<std::string, FunctionType*> builtInFunctions;
private:
    bool isTopLevelScope();

    friend class Scope;
};

bool Variables::has(const std::string &name) {
    return vars.find(name) != vars.end();
}

Variable Variables::get(const std::string &name) {
    auto it = vars.find(name);
    if (it == vars.end()) {
        throw std::runtime_error("Variable does not exist");
    }
    return it->second;
}

void Variables::set(const std::string &name, Variable value) {
    vars[name] = value;
}

Scope::Scope()
    : impl(std::make_shared<ScopeImpl>())
    {}

Variable Scope::execute() {
    auto res = impl->instruction.execute();
    switch (impl->type) {
    case ScopeType::TOP_LEVEL:
        break;
    case ScopeType::IF:
        if (res->to_bool()) {
            for (auto child: impl->children) {
                res = child.execute();
            }
        }
        break;
    case ScopeType::WHILE:
        while (res->to_bool()) {
            for (auto child: impl->children) {
                res = child.execute();
            }
        }
        break;
    default: {
        throw std::runtime_error("unsupported scope type");
    }
    }

    return res;
}

void Scope::addChild(const Scope &child) {
    impl->children.push_back(child);
    child.impl->parent = impl;
    child.impl->type = ScopeType::ORDINARY_LINE;
}

void Scope::addBuiltInFunction(const std::string &name, FunctionType function) {
    impl->builtInFunctions[name] = function;
}

bool ScopeImpl::isTopLevelScope() {
    return type == ScopeType::TOP_LEVEL;
}

Variable Scope::call(const std::string &name, const InstructionParams &params) {
    auto scope = scopeWithVariable(name);
    if (scope) {
        if (scope->vars.has(name)) {
            throw std::runtime_error("Functions stored in variables are not implemented");
        }

        auto it = scope->builtInFunctions.find(name);
        if (it != scope->builtInFunctions.end()) {
            return it->second(params);
        }
    }

    throw std::runtime_error("Function not defined");
}

#define REMOVE_BUILT_IN(SCOPE, NAME) \
        auto it = SCOPE->builtInFunctions.find(NAME); \
        if (it != SCOPE->builtInFunctions.end()) { \
            SCOPE->builtInFunctions.erase(it); \
        }

void Scope::setVariable(const std::string &name, Variable value) {
    auto scope = scopeWithVariable(name);

    // If the variable exists in one of the scopes - update it there
    if (scope) {
        scope->vars.set(name, value);
        REMOVE_BUILT_IN(scope, name);
    }
    // Otherwise create it in the current scope
    else {
        impl->vars.set(name, value);
        REMOVE_BUILT_IN(impl, name);
    }
}

Variable Scope::getVariable(const std::string &name) {
    auto scope = scopeWithVariable(name);

    if (!scope) {
        throw std::runtime_error("Variable not found " + name);
    }

    return scope->vars.get(name);
}

std::shared_ptr<ScopeImpl> Scope::scopeWithVariable(const std::string &name) {
    std::shared_ptr<ScopeImpl> curr = impl;

    while (true) {
        if (curr->vars.has(name)) {
            return curr;
        }

        auto it = curr->builtInFunctions.find(name);
        if (it != curr->builtInFunctions.end()) {
            return curr;
        }

        if (curr->isTopLevelScope()) {
            break;
        }

        auto parent = curr->parent.lock();
        if (!parent) {
            throw std::runtime_error("Parent scope already destroyed");
        }
        curr = parent;
    }

    return nullptr;
}

} // namespace MiniPython