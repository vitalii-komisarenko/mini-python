#include "Scope.h"

#include <stdexcept>

namespace MiniPython {

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

} // namespace MiniPython
