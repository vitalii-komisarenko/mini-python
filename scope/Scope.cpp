#include "Scope.h"

#include <stdexcept>

namespace MiniPython {

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

Variable _Scope::execute() {
    auto res = instruction.execute();
    switch (type) {
    case ScopeType::TOP_LEVEL:
        break;
    case ScopeType::IF:
        if (res->to_bool()) {
            for (auto child: children) {
                res = child->execute();
            }
        }
        break;
    case ScopeType::WHILE:
        while (res->to_bool()) {
            for (auto child: children) {
                res = child->execute();
            }
        }
        break;
    default: {
        throw std::runtime_error("unsupported scope type");
    }
    }

    return res;
}

void _Scope::addChild(Scope child) {
    children.push_back(child);
}

Variable _Scope::call(const std::string &name, const InstructionParams &params)
{
    _Scope *curr = this;
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

bool _Scope::isTopLevelScope() {
    return type != ScopeType::TOP_LEVEL;
}

} // namespace MiniPython
