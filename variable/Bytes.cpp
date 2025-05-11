#include "Variable.h"
#include "src/StringFormatting.h"
#include "Utils.h"

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
        for (IntType i = 0; i < other_casted->get_value(); ++i) {
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

static std::string char_to_str(unsigned char ch) {
    if (ch == '\t') {
        return "\\t";
    }
    if (ch == '\n') {
        return "\\n";
    }
    if (ch == '\r') {
        return "\\r";
    }
    if ((ch < 0x20) || (ch > 0x7e)) {
        return "\\x" + byte_to_hex(ch);
    }
    return std::string(1, ch);
}

std::string Bytes::to_str() {
    bool has_single_quote = value.find("'") != std::string::npos;
    bool has_double_quote = value.find("\"") != std::string::npos;

    char quote_type = (has_single_quote && !has_double_quote) ? '"' : '\'';

    std::string between_quotes;
    for (char ch: value) {
        if (ch == quote_type) {
            between_quotes += '\\';
            between_quotes += ch;
        }
        else {
            between_quotes += char_to_str(ch);
        }
    }
    return std::string("b") + quote_type + between_quotes + quote_type;
}

Variable Bytes::to_bytes_variable() {
    return NEW_BYTES(value);
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
