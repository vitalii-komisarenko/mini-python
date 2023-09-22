#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

static bool ch_is_lower(char ch) {
    return ('a' <= ch) && (ch <= 'z');
}

static bool ch_is_upper(char ch) {
    return ('A' <= ch) && (ch <= 'Z');
}

static bool ch_is_cased(char ch) {
    return ch_is_lower(ch) || ch_is_upper(ch);
}

static char ch_to_lower(char ch) {
    if (('A' <= ch) && (ch <= 'Z')) {
        return ch + 32;
    }
    return ch;
}

static char ch_to_upper(char ch) {
    if (('a' <= ch) && (ch <= 'z')) {
        return ch - 32;
    }
    return ch;
}

static Variable encode_string(const std::string &value) {
    return std::make_shared<MiniPython::StringVariable>(value);
}

extern Variable execute_instruction(std::shared_ptr<Instruction> instr, Scope *scope);

#define DECODE_STRING(index) std::dynamic_pointer_cast<StringVariable>(execute_instruction(params[index], scope))->value
#define DECODE_INT(index) std::dynamic_pointer_cast<IntVariable>(execute_instruction(params[index], scope))->value

static Variable islower(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    bool has_cased_char = false;
    bool has_lower_char = false;
    for (size_t i = 0; i < orig_value.size(); ++i) {
        char ch = orig_value[i];
        if (ch_is_cased(ch)) {
            has_cased_char = true;
        }
        if (ch_is_lower(ch)) {
            has_lower_char = true;
        }
    }
    bool res = has_cased_char && (!has_lower_char);
    return std::make_shared<BoolVariable>(res);
}

static Variable isupper(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    bool has_cased_char = false;
    bool has_upper_char = false;
    for (size_t i = 0; i < orig_value.size(); ++i) {
        char ch = orig_value[i];
        if (ch_is_cased(ch)) {
            has_cased_char = true;
        }
        if (ch_is_upper(ch)) {
            has_upper_char = true;
        }
    }
    bool res = has_cased_char && (!has_upper_char);
    return std::make_shared<BoolVariable>(res);
}

static Variable lower(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    std::string res;
    for (size_t i = 0; i < orig_value.size(); ++i) {
        res += ch_to_lower(orig_value[i]);
    }
    return encode_string(res);
}

static Variable upper(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    std::string res;
    for (size_t i = 0; i < orig_value.size(); ++i) {
        res += ch_to_upper(orig_value[i]);
    }
    return encode_string(res);
}

static Variable capitalize(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    std::string res;
    for (size_t i = 0; i < orig_value.size(); ++i) {
        if (i == 0) {
            res += ch_to_upper(orig_value[i]);
        }
        else {
            res += ch_to_lower(orig_value[i]);
        }
    }
    return encode_string(res);
}

static Variable swapcase(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    std::string res;
    for (size_t i = 0; i < orig_value.size(); ++i) {
        char ch = orig_value[i];
        if (ch_is_lower(ch)) {
            res += ch_to_upper(ch);
        }
        else if (ch_is_upper(ch)) {
            res += ch_to_lower(ch);
        }
        else {
            res += ch;
        }
    }

    return encode_string(res);
}

static std::string create_padding(const std::string &str_to_pad, int desired_len, char padding_char) {
    int padding_len = desired_len > str_to_pad.size() ? desired_len - str_to_pad.size() : 0;
    return std::string(padding_len, padding_char);
}

static Variable ljust(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    int desired_len = DECODE_INT(1);
    char padding_char = (params.size() == 3) ? DECODE_STRING(2)[0] : ' ';

    std::string padding = create_padding(orig_value, desired_len, padding_char);
    return encode_string(padding + orig_value);
}

static Variable rjust(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    int desired_len = DECODE_INT(1);
    char padding_char = (params.size() == 3) ? DECODE_STRING(2)[0] : ' ';

    std::string padding = create_padding(orig_value, desired_len, padding_char);
    return encode_string(orig_value + padding);
}

static Variable zfill(const InstructionParams& params, Scope *scope) {
    std::string orig_value = DECODE_STRING(0);
    int desired_len = DECODE_INT(1);
    std::string res;
    int start_pos = 0;

    if (orig_value.size() >= desired_len) {
        return encode_string(orig_value);
    }

    if ((orig_value.size() > 0) && ((orig_value[0] == '+') || (orig_value[0] == '-'))) {
        start_pos = 1;
        res += orig_value[0];
    }

    res += create_padding(orig_value, desired_len, '0');
    res += orig_value.substr(start_pos);

    return encode_string(res);
}

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
