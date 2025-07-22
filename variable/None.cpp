#include "Variable.h"

namespace MiniPython {

VariableType NoneVariable::get_type() {
    return VariableType::NONE;
}

bool NoneVariable::to_bool() {
    return false;
}

std::string NoneVariable::to_str() {
    return "None";
}

bool NoneVariable::equal(const Variable &other) {
    return other->get_type() == VariableType::NONE;
}

bool NoneVariable::strictly_equal(const Variable &other) {
    if (get_type() != other->get_type()) {
        return false;
    }

    return true;
}

} // namespace MiniPython
