#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

FloatVariable::FloatVariable(FloatType _value): value(_value) {}

VariableType FloatVariable::get_type() {
    return VariableType::FLOAT;
}

FloatVariable::FloatType FloatVariable::get_value() {
    return value;
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
        return std::make_shared<FloatVariable>(value / other_casted->get_value());
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return div(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        return std::make_shared<FloatVariable>(value / other_casted->value);
    }
    default:
        throw std::runtime_error("Can't divide float by that");
    }
}

Variable FloatVariable::mod(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        if (other_casted->get_value() == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        std::runtime_error("Not implemented: float % int");
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mod(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        auto other_casted = std::dynamic_pointer_cast<FloatVariable>(other);
        if (other_casted->value == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        std::runtime_error("Not implemented: float % float");
    }
    default:
        throw std::runtime_error("Can't do modular arithmetic with float and that type");
    }
}

Variable FloatVariable::pow(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);

        FloatType result = 1;

        // Note that according to Python on my computer, 0 ** 0 == 1,
        // so it is apparently NOT a corner case
        if (other_casted->get_value() >= 0) {
            // TODO: faster exponentation
            for (size_t i = 0; i < other_casted->get_value(); ++i) {
                result *= this->value;
            }
        }
        else {
            throw std::runtime_error("Not implemented - negative powers of float");
        }
        return std::make_shared<FloatVariable>(result);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return pow(other_casted->toIntVar());
    }
    case VariableType::FLOAT: {
        throw std::runtime_error("Not implemented - float ** float");
    }
    default:
        throw std::runtime_error("Can't raise float into power of that type");
    }
}

bool FloatVariable::to_bool() {
    return value != 0;
}

std::string FloatVariable::to_str() {
    return std::to_string(value);
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
    case VariableType::NONE:
    case VariableType::STRING:
        throw std::runtime_error("Can't compare float with this type");
    default:
        return false;
    }
}

} // namespace MiniPython
