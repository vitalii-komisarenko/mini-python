#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

IntVariable::IntVariable(IntType _value): value(_value) {}

VariableType IntVariable::get_type() {
    return VariableType::INT;
}

Variable IntVariable::add(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return std::make_shared<IntVariable>(value + other_casted->value);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return add(other_casted->toIntVar());
    }
    // TODO: float
    default:
        throw std::runtime_error("Can't add this to int");
    }
}

Variable IntVariable::sub(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return std::make_shared<IntVariable>(value - other_casted->value);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return sub(other_casted->toIntVar());
    }
    // TODO: float
    default:
        throw std::runtime_error("Can't substract this from int");
    }
}

Variable IntVariable::mul(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return std::make_shared<IntVariable>(value * other_casted->value);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mul(other_casted->toIntVar());
    }
    // TODO: float
    // TODO: str
    // TODO: list
    default:
        throw std::runtime_error("Can't multiply that with int");
    }
}

Variable IntVariable::div(const Variable &other) {
    switch (other->get_type()) {
    // TODO: int
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return div(other_casted->toIntVar());
    }
    // TODO: float
    default:
        throw std::runtime_error("Can't divide int by that");
    }
}

Variable IntVariable::mod(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        if (other_casted->value == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        return std::make_shared<IntVariable>(value % other_casted->value);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mod(other_casted->toIntVar());
    }
    // TODO: float
    default:
        throw std::runtime_error("Can't do modular arithmetic with int and that type");
    }
}

Variable IntVariable::pow(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);

        IntType result = 1;

        // Note that according to Python on my computer, 0 ** 0 == 1,
        // so it is apparently NOT a corner case
        if (other_casted->value >= 0) {
            // TODO: faster exponentation
            for (size_t i = 0; i < other_casted->value; ++i) {
                result *= this->value;
            }
        }
        else {
            throw std::runtime_error("Not implemented - negative powers of int");
        }
        return std::make_shared<IntVariable>(result);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return pow(other_casted->toIntVar());
    }
    // TODO: float
    default:
        throw std::runtime_error("Can't raise int into power of that type");
    }
}

bool IntVariable::to_bool() {
    return value != 0;
}

std::string IntVariable::to_str() {
    return std::to_string(value);
}

bool IntVariable::equal(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return this->value == other_casted->value;
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return equal(other_casted->toIntVar());
    }
    // TODO: float
    default:
        return false;
    }
}

bool IntVariable::less(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);
        return this->value < other_casted->value;
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return less(other_casted->toIntVar());
    }
    // TODO: float
    default:
        return false;
    }
}

} // namespace MiniPython
