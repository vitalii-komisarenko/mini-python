#include "Scope.h"

#include <stdexcept>

namespace MiniPython {

class ScopeImpl {
public:
    ScopeImpl()
        : type(ScopeType::TOP_LEVEL) {}

    ScopeType type;

    Instruction instruction;
    Variable call(const std::string &name, const InstructionParams &params);
    Variables vars;
    std::weak_ptr<ScopeImpl> parent;
    std::vector<Scope> children;
    std::unordered_map<std::string, Scope::Func*> builtInFunctions;
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
}

void Scope::addBuiltInFunction(const std::string &name, Func function) {
    impl->builtInFunctions[name] = function;
}

Variable ScopeImpl::call(const std::string &name, const InstructionParams &params)
{
    ScopeImpl *curr = this;
    while (true) {
        if (curr->vars.has(name)) {
            throw std::runtime_error("Functions stored in variables are not implemented");
        }

        auto it = curr->builtInFunctions.find(name);
        if (it != curr->builtInFunctions.end()) {
            return it->second(params);
        }

        if (!curr->isTopLevelScope()) {
            auto parent = curr->parent.lock();
            if (!parent) {
                throw std::runtime_error("Parent scope already destroyed");
            }
            curr = parent.get();
        }
    }
}

bool ScopeImpl::isTopLevelScope() {
    return type != ScopeType::TOP_LEVEL;
}

Variable Scope::call(const std::string &name, const InstructionParams &params) {
    return impl->call(name, params);
}

} // namespace MiniPython
