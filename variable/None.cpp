#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

VariableType NoneVariable::get_type() {
    return VariableType::NONE;
}

Variable NoneVariable::add(const Variable &other) {
    throw std::runtime_error("Can't add to None");
}

Variable NoneVariable::sub(const Variable &other) {
    throw std::runtime_error("Can't substract from None");
}

Variable NoneVariable::mul(const Variable &other) {
    throw std::runtime_error("Can't multiply None");
}

Variable NoneVariable::div(const Variable &other) {
    throw std::runtime_error("Can't divide None");
}

Variable NoneVariable::int_div(const Variable &other) {
    throw std::runtime_error("Can't divide None");
}

Variable NoneVariable::mod(const Variable &other) {
    throw std::runtime_error("Can't do modular arithmetic on None");
}

Variable NoneVariable::pow(const Variable &other) {
    throw std::runtime_error("Can't raise None to any power");
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

bool NoneVariable::less(const Variable &other) {
    throw std::runtime_error("Can't use < or > with None");
}

} // namespace MiniPython
