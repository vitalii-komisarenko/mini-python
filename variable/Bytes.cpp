#include "Variable.h"
#include "src/StringFormatting.h"

#include <stdexcept>

namespace MiniPython {

/*
 * Standard Variable API
 */

Bytes::Bytes(const StringType &_value): StringVariable(_value) {}

VariableType Bytes::get_type() {
    return VariableType::BYTES;
}

Bytes::StringType Bytes::get_value() {
    return value;
}

Variable Bytes::add(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::BYTES: {
        auto other_casted = std::dynamic_pointer_cast<Bytes>(other);
        return std::make_shared<Bytes>(value + other_casted->value);
    }
    default:
        throw std::runtime_error("Can't add this to bytes");
    }
}

Variable Bytes::mul(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);

        std::string result = {};
        for (IntVariable::IntType i = 0; i < other_casted->get_value(); ++i) {
            result += value;
        }
        return std::make_shared<Bytes>(result);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mul(other_casted->toIntVar());
    }
    default:
        throw std::runtime_error("Can't multiply bytes by that");
    }
}

Variable Bytes::mod(const Variable &other) {
    if (is_tuple(other)) {
        return NEW_BYTES(PercentFormatter(to_str()).format(VAR_TO_LIST(other)));
    }
    else {
        std::vector<Variable> vec;
        vec.push_back(other);
        return NEW_BYTES(PercentFormatter(to_str()).format(vec));
    }
}

std::string Bytes::to_str() {
    return std::string("b'") + Bytes::to_str() + "'";
}

bool Bytes::equal(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::BYTES: {
        auto other_casted = std::dynamic_pointer_cast<Bytes>(other);
        return this->value == other_casted->value;
    }
    default:
        return false;
    }
}

bool Bytes::less(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::BYTES: {
        auto other_casted = std::dynamic_pointer_cast<Bytes>(other);
        return this->value < other_casted->value;
    }
    default:
        throw std::runtime_error("Can't use < and > with bytes an non-bytes");
    }
}

bool Bytes::strictly_equal(const Variable &other) {
    if (get_type() != other->get_type()) {
        return false;
    }

    return value == std::dynamic_pointer_cast<Bytes>(other)->value;
}

} // namespace MiniPython
