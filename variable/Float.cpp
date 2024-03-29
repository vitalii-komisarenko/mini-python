#include "Variable.h"

#include <cmath>
#include <stdexcept>

namespace MiniPython {

FloatVariable::FloatVariable(FloatType _value): value(_value) {}

VariableType FloatVariable::get_type() {
    return VariableType::FLOAT;
}

FloatVariable::FloatType FloatVariable::get_value() {
    return value;
}

Variable FloatVariable::toFloatVar() {
    return std::make_shared<FloatVariable>(value);
}

Variable FloatVariable::add(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return std::make_shared<FloatVariable>(value + other_casted->get_value());
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return add(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        return std::make_shared<FloatVariable>(value + other_casted->value);
    }
    default:
        throw std::runtime_error("Can't add this to float");
    }
}

Variable FloatVariable::sub(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return std::make_shared<FloatVariable>(value - other_casted->get_value());
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return sub(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        return std::make_shared<FloatVariable>(value - other_casted->value);
    }
    default:
        throw std::runtime_error("Can't substract this from float");
    }
}

Variable FloatVariable::mul(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return std::make_shared<FloatVariable>(value * other_casted->get_value());
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mul(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        return std::make_shared<FloatVariable>(value * other_casted->value);
    }
    default:
        throw std::runtime_error("Can't multiply that with float");
    }
}

Variable FloatVariable::div(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        if (other_casted->get_value() == 0) {
            throw std::runtime_error("Division by zero");
        }
        return std::make_shared<FloatVariable>(value / other_casted->get_value());
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return div(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        if (other_casted->get_value() == 0) {
            throw std::runtime_error("Division by zero");
        }
        return std::make_shared<FloatVariable>(value / other_casted->value);
    }
    default:
        throw std::runtime_error("Can't divide float by that");
    }
}

Variable FloatVariable::int_div(const Variable &other) {
    auto float_div = div(other);
    auto float_div_converted = std::dynamic_pointer_cast<FloatVariable>(float_div);
    return std::make_shared<FloatVariable>(std::floor(float_div_converted->get_value()));
}

Variable FloatVariable::mod(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return mod(other_casted->toFloatVar());
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mod(other_casted->toFloatVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        if (other_casted->value == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        // x%y = x - (x//y)*y
        auto x = value;
        auto y = other_casted->get_value();

        auto result = x - (int64_t(x / y)) * y;

        if (x < 0 && y > 0 && result != 0) {
            result += y;
        }
        if (x > 0 && y < 0 && result != 0) {
            result += y;
        }

        return std::make_shared<FloatVariable>(result);
    }
    default:
        throw std::runtime_error("Can't do modular arithmetic with float and that type");
    }
}

Variable FloatVariable::pow(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return pow(other_casted->toFloatVar());
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return pow(other_casted->toFloatVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        if (value == 0 && other_casted->value < 0) {
            throw std::runtime_error("ZeroDivisionError: 0.0 cannot be raised to a negative power");
        }
        return std::make_shared<FloatVariable>(std::pow(value, other_casted->value));
    }
    default:
        throw std::runtime_error("Can't raise float to power of that type");
    }
}

bool FloatVariable::to_bool() {
    return value != 0;
}

std::string FloatVariable::to_str() {
    auto str = std::to_string(value);
    // C++ adds too many zeros in the fractional part (e.g. 123.000000 while Python uses 123.0)
    size_t dot_pos = str.find('.');
    size_t trailing_zero_start = str.length();
    for (size_t i = str.length() - 1; (i >= 0) && (str[i] == '0'); --i) {
        trailing_zero_start = i;
    }
    // Ensure that there is at least one digit in fractional part (e.g. 123.0, not 123)
    size_t new_length = trailing_zero_start > dot_pos + 1 ? trailing_zero_start : trailing_zero_start + 1;
    if (new_length < str.length()) {
        str.resize(new_length);
    }
    return str;
}

bool FloatVariable::equal(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return this->value == other_casted->get_value();
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return equal(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        return this->value == other_casted->value;
    }
    default:
        return false;
    }
}

bool FloatVariable::less(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return this->value < other_casted->get_value();
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return less(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        return this->value < other_casted->value;
    }
    default:
        throw std::runtime_error("Can't compare float with this type");
    }
}

bool FloatVariable::strictly_equal(const Variable &other) {
    if (get_type() != other->get_type()) {
        return false;
    }

    return value == std::dynamic_pointer_cast<FloatVariable>(other)->value;
}

} // namespace MiniPython
