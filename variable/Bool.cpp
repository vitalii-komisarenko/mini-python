#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

static const Variable Zero = std::make_shared<IntVariable>(0);
static const Variable One = std::make_shared<IntVariable>(1);

Variable BoolVariable::toIntVar() {
    return value ? One : Zero;
}

Variable BoolVariable::toFloatVar() {
    return std::make_shared<FloatVariable>(value ? 1 : 0);
}

BoolVariable::BoolVariable(bool _value): value(_value) {}

VariableType BoolVariable::get_type() {
    return VariableType::BOOL;
}

Variable BoolVariable::add(const Variable &other) {
    return toIntVar()->add(other);
}

Variable BoolVariable::sub(const Variable &other) {
    return toIntVar()->sub(other);
}

Variable BoolVariable::mul(const Variable &other) {
    return toIntVar()->mul(other);
}

Variable BoolVariable::div(const Variable &other) {
    return toIntVar()->div(other);
}

Variable BoolVariable::mod(const Variable &other) {
    return toIntVar()->mod(other);
}

Variable BoolVariable::pow(const Variable &other) {
    return toIntVar()->pow(other);
}

bool BoolVariable::to_bool() {
    return value;
}

std::string BoolVariable::to_str() {
    return value ? "True" : "False";
}

bool BoolVariable::equal(const Variable &other) {
    return toIntVar()->equal(other);
}

bool BoolVariable::less(const Variable &other) {
    return toIntVar()->less(other);
}

} // namespace MiniPython
