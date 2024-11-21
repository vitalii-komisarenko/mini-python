#include "Variable.h"

namespace MiniPython {

VariableType ObjectNotFoundVariable::get_type() {
    return VariableType::OBJECT_NOT_FOUND;
}

Variable ObjectNotFoundVariable::add(const Variable &other) { return NONE; }
Variable ObjectNotFoundVariable::sub(const Variable &other) { return NONE; }
Variable ObjectNotFoundVariable::mul(const Variable &other) { return NONE; }
Variable ObjectNotFoundVariable::div(const Variable &other) { return NONE; }
Variable ObjectNotFoundVariable::mod(const Variable &other) { return NONE; }
Variable ObjectNotFoundVariable::pow(const Variable &other) { return NONE; }
Variable ObjectNotFoundVariable::int_div(const Variable &other) { return NONE; }

bool ObjectNotFoundVariable::to_bool() { return false; }
std::string ObjectNotFoundVariable::to_str() { return ""; }

bool ObjectNotFoundVariable::equal(const Variable &other) {
    return other->get_type() == VariableType::OBJECT_NOT_FOUND;
}

bool ObjectNotFoundVariable::less(const Variable &other) { return false; }
bool ObjectNotFoundVariable::strictly_equal(const Variable &other) { return false; }

} // namespace MiniPython
