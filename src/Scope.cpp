#include "Scope.h"
#include "LineLevelParser.h"

#include <array>
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

void Variables::set(const std::string &name, Variable value) {
    vars[name] = value;
}

Scope::Scope()
    : impl(std::make_shared<ScopeImpl>())
    {}

std::shared_ptr<Scope> makeScope(const LineTree &lineTree, bool isTopLevel) {
    auto scope = std::make_shared<Scope>();
    scope->impl = std::make_shared<ScopeImpl>();

    auto tokenList = tokenizeLine(lineTree.value);

    ScopeType scopeType = isTopLevel ? ScopeType::TOP_LEVEL : ScopeType::ORDINARY_LINE;
    if (tokenList.size() > 0 && tokenList[0].type == TokenType::IDENTIFIER) {
        if (tokenList[0].value == "if") {
            scopeType = ScopeType::IF;
            tokenList.erase(tokenList.begin());
        }
        else if (tokenList[0].value == "while") {
            scopeType = ScopeType::WHILE;
            tokenList.erase(tokenList.begin());
        }
    }

    scope->impl->type = scopeType;
    scope->impl->instruction = Instruction::fromTokenList(tokenList);
    for (const auto& childTree : lineTree.children) {
        auto newChild = makeScope(*childTree, false);
        newChild->parentScope = scope;
        scope->impl->children.push_back(*newChild); // TODO: push shared_ptr, not a copy
    }

    return scope;
}

Variable Scope::execute() {
    auto res = impl->instruction.execute(std::make_shared<Scope>(*this));
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

bool ScopeImpl::isTopLevelScope() {
    return type == ScopeType::TOP_LEVEL;
}

Variable Scope::call(const std::string &name, const InstructionParams &params) {
    auto scope = scopeWithVariable(name);
    if (!scope) {
        throw std::runtime_error("Function not defined");
    }

    if (!scope->vars.has(name)) {
        throw std::runtime_error("Variable is not in scope");
    }

    auto var = scope->vars.get(name);
    if (var->get_type() != VariableType::FUNCTION) {
        throw std::runtime_error("Cannot call a variable that is not a function");
    }

    auto func = std::dynamic_pointer_cast<FunctionVariable>(var);
    return func->call(params, std::make_shared<Scope>(*this));
}

void Scope::setVariable(const std::string &name, Variable value) {
    static std::array<std::string, 3> reserved_names = {"None", "False", "True"};
    if (std::find(std::begin(reserved_names), std::end(reserved_names), name) != std::end(reserved_names)) {
        throw std::runtime_error("Cannot assing to variable '" + name + "': the name is reserved");
    }

    auto scope = scopeWithVariable(name);

    // If the variable exists in one of the scopes - update it there
    if (scope) {
        scope->vars.set(name, value);
    }
    // Otherwise create it in the current scope
    else {
        impl->vars.set(name, value);
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
