#include "StandardFunctions.h"

#include <iostream>
#include <stdexcept>

namespace MiniPython::StandardFunctions {

static auto None = std::static_pointer_cast<GenericVariable>(std::make_shared<NoneVariable>());

Variable print(const InstructionParams &params, Scope *scope) {
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << params[i]->execute(scope)->to_str();
        bool is_last = i == params.size() - 1;
        std::cout << (is_last ? "\n" : " ");
    }
    return None;
}

Variable min(const InstructionParams &params, Scope *scope) {
    if (params.size() == 0) {
        throw std::runtime_error("TypeError: min expected at least 1 argument, got 0");
    }

    auto res = std::static_pointer_cast<GenericVariable>(params[0]->execute(scope));

    for (size_t i = 0; i < params.size(); ++i) {
        auto var = params[i]->execute(scope);
        if (var->less(res)) {
            res = var;
        }
    }

    return res;
}

Variable max(const InstructionParams &params, Scope *scope) {
    if (params.size() == 0) {
        throw std::runtime_error("TypeError: min expected at least 1 argument, got 0");
    }

    auto res = std::static_pointer_cast<GenericVariable>(params[0]->execute(scope));

    for (size_t i = 0; i < params.size(); ++i) {
        auto var = params[i]->execute(scope);
        if (!var->less(res)) {
            res = var;
        }
    }

    return res;
}

} // namespace MiniPython::StandardFunctions
