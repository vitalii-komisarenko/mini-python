#include "StandardFunctions.h"

#include <iostream>

namespace MiniPython::StandardFunctions {

static auto None = std::static_pointer_cast<GenericVariable>(std::make_shared<NoneVariable>());

Variable print(const InstructionParams &params, std::shared_ptr<Scope> scope) {
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << params[i]->execute(scope);
        bool is_last = i == params.size() - 1;
        std::cout << (is_last ? "\n" : " ");
    }
    return None;
}

} // namespace MiniPython::StandardFunctions
