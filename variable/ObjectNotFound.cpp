#include "Variable.h"

namespace MiniPython {

VariableType ObjectNotFoundVariable::get_type() {
    return VariableType::OBJECT_NOT_FOUND;
}

bool ObjectNotFoundVariable::equal(const Variable &other) {
    return other->get_type() == VariableType::OBJECT_NOT_FOUND;
}

} // namespace MiniPython
