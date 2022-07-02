#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

StringVariable::StringVariable(const StringType &_value): value(_value) {}

VariableType StringVariable::get_type() {
    return VariableType::STRING;
}

StringVariable::StringType StringVariable::get_value() {
    return value;
}

Variable StringVariable::add(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::STRING: {
        auto other_casted = std::dynamic_pointer_cast<StringVariable>(other);
        return std::make_shared<StringVariable>(value + other_casted->value);
    }
    default:
        throw std::runtime_error("Can't add this to string");
    }
}

Variable StringVariable::sub(const Variable &other) {
    throw std::runtime_error("Can't substract anything from string");
}

Variable StringVariable::mul(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);

        std::string result = {};
        for (IntVariable::IntType i = 0; i < other_casted->get_value(); ++i) {
            result += value;
        }
        return std::make_shared<StringVariable>(result);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mul(other_casted->toIntVar());
    }
    default:
        throw std::runtime_error("Can't multiply string by that");
    }
}

Variable StringVariable::div(const Variable &other) {
    throw std::runtime_error("Can't divide string by anything");
}

Variable StringVariable::int_div(const Variable &other) {
    throw std::runtime_error("Can't divide string by anything");
}

Variable StringVariable::mod(const Variable &other) {
    throw std::runtime_error("Can't do modular arithmetic on string");
}

Variable StringVariable::pow(const Variable &other) {
    throw std::runtime_error("Can't raise string to any power");
}

bool StringVariable::to_bool() {
    return value.size() != 0;
}

std::string StringVariable::to_str() {
    return value;
}

bool StringVariable::equal(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::STRING: {
        auto other_casted = std::dynamic_pointer_cast<StringVariable>(other);
        return this->value == other_casted->value;
    }
    default:
        return false;
    }
}

bool StringVariable::less(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::STRING: {
        auto other_casted = std::dynamic_pointer_cast<StringVariable>(other);
        return this->value < other_casted->value;
    }
    default:
        throw std::runtime_error("Can't use < and > with string an non-string");
    }
}

bool StringVariable::strictly_equal(const Variable &other) {
    if (get_type() != other->get_type()) {
        return false;
    }

    return value == std::dynamic_pointer_cast<StringVariable>(other)->value;
}

} // namespace MiniPython
