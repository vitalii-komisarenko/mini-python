#include "Variable.h"
#include "src/StringFormatting.h"

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

static bool ch_is_alpha(char ch) {
    return (('a' <= ch) && (ch <= 'z')) || (('A' <= ch) && (ch <= 'Z'));
}

static bool ch_is_numeric(char ch) {
    return ('0' <= ch) && (ch <= '9');
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

static Variable isalpha(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    if (str.size() == 0) {
        return std::make_shared<BoolVariable>(false);
    }
    for (size_t i = 0; i < str.size(); ++i) {
        if (!ch_is_alpha(str[i])) {
            return std::make_shared<BoolVariable>(false);
        }
    }
    return std::make_shared<BoolVariable>(true);
}

static Variable isascii(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    if (str.size() == 0) {
        return std::make_shared<BoolVariable>(true);
    }
    for (size_t i = 0; i < str.size(); ++i) {
        if ((str[i] < 0) || (str[i] > 0xFF)) {
            return std::make_shared<BoolVariable>(false);
        }
    }
    return std::make_shared<BoolVariable>(true);
}

static Variable isdecimal(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    if (str.size() == 0) {
        return std::make_shared<BoolVariable>(false);
    }
    for (size_t i = 0; i < str.size(); ++i) {
        if (!ch_is_numeric(str[i])) {
            return std::make_shared<BoolVariable>(false);
        }
    }
    return std::make_shared<BoolVariable>(true);
}

static Variable isalnum(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    if (str.size() == 0) {
        return std::make_shared<BoolVariable>(false);
    }
    for (size_t i = 0; i < str.size(); ++i) {
        if (!ch_is_alpha(str[i]) && !ch_is_numeric(str[i])) {
            return std::make_shared<BoolVariable>(false);
        }
    }
    return std::make_shared<BoolVariable>(true);
}

static Variable isdigit(const InstructionParams& params, Scope *scope) {
    return isdecimal(params, scope);
}

static Variable isnumeric(const InstructionParams& params, Scope *scope) {
    return isdecimal(params, scope);
}

static Variable isspace(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    if (str.size() == 0) {
        return std::make_shared<BoolVariable>(false);
    }
    for (size_t i = 0; i < str.size(); ++i) {
        if ((str[i] != ' ') && (str[i] != '\t')) {
            return std::make_shared<BoolVariable>(false);
        }
    }
    return std::make_shared<BoolVariable>(true);
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

bool str_starts_with(const std::string &str, const std::string &substr) {
    if (substr.size() > str.size()) {
        return false;
    }
    return str.rfind(substr, 0) == 0;
}

bool str_ends_with(const std::string &str, const std::string substr) {
    if (substr.size() > str.size()) {
        return false;
    }
    return str.find(substr, str.size() - substr.size()) == str.size() - substr.size();
}

static Variable startswith(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string substr = DECODE_STRING(1);
    bool res = str_starts_with(str, substr);
    return std::make_shared<BoolVariable>(res);
}

static Variable endswith(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string substr = DECODE_STRING(1);
    bool res = str_ends_with(str, substr);
    return std::make_shared<BoolVariable>(res);
}

static Variable removeprefix(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string prefix = DECODE_STRING(1);
    if (!str_starts_with(str, prefix)) {
        return encode_string(str);
    }
    return encode_string(str.substr(prefix.size()));
}

static Variable removesuffix(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string suffix = DECODE_STRING(1);
    if (!str_ends_with(str, suffix)) {
        return encode_string(str);
    }
    return encode_string(str.substr(0, str.size() - suffix.size()));
}

bool str_contains_char(const std::string &str, char ch) {
    return str.find(ch) != std::string::npos;
}

std::string str_lstrip(const std::string &str, const std::string &chars_to_strip) {
    int first_char_not_to_strip = -1;
    for (size_t i = 0; i < str.size(); ++i) {
        if (!str_contains_char(chars_to_strip, str[i])) {
            first_char_not_to_strip = i;
            break;
        }
    }
    if (first_char_not_to_strip == -1) {
        return "";
    }
    return str.substr(first_char_not_to_strip);
}

std::string str_rstrip(const std::string &str, const std::string &chars_to_strip) {
    int first_char_not_to_strip = -1;
    for (int i = str.size() - 1; i >= 0; --i) {
        if (!str_contains_char(chars_to_strip, str[i])) {
            first_char_not_to_strip = i;
            break;
        }
    }
    if (first_char_not_to_strip == -1) {
        return "";
    }
    return str.substr(0, first_char_not_to_strip);
}

static Variable lstrip(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string chars = (params.size() == 2) ? DECODE_STRING(1) : " \t";
    return encode_string(str_lstrip(str, chars));
}

static Variable rstrip(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string chars = (params.size() == 2) ? DECODE_STRING(1) : " \t";
    return encode_string(str_rstrip(str, chars));
}

static Variable strip(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string chars = (params.size() == 2) ? DECODE_STRING(1) : " \t";
    return encode_string(str_lstrip(str_rstrip(str, chars), chars));
}

static size_t _find(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string substr = DECODE_STRING(1);
    int start = params.size() > 2 ? DECODE_INT(2) : 0;
    int end = params.size() > 3 ? DECODE_INT(3) : str.size();
    return str.substr(start, end).find(substr);
}

static Variable find(const InstructionParams& params, Scope *scope) {
    size_t pos = _find(params, scope);
    if (pos == std::string::npos) {
        return std::make_shared<IntVariable>(-1);
    }
    return std::make_shared<IntVariable>(pos);
}

static Variable index(const InstructionParams& params, Scope *scope) {
    size_t pos = _find(params, scope);
    if (pos == std::string::npos) {
        throw std::runtime_error("ValueError: index: substring not found");
    }
    return std::make_shared<IntVariable>(pos);
}

static size_t _rfind(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string substr = DECODE_STRING(1);
    int start = params.size() > 2 ? DECODE_INT(2) : 0;
    int end = params.size() > 3 ? DECODE_INT(3) : str.size();
    return str.substr(start, end).rfind(substr);
}

static Variable rfind(const InstructionParams& params, Scope *scope) {
    size_t pos = _rfind(params, scope);
    if (pos == std::string::npos) {
        return std::make_shared<IntVariable>(-1);
    }
    return std::make_shared<IntVariable>(pos);
}

static Variable rindex(const InstructionParams& params, Scope *scope) {
    size_t pos = _rfind(params, scope);
    if (pos == std::string::npos) {
        throw std::runtime_error("ValueError: rindex: substring not found");
    }
    return std::make_shared<IntVariable>(pos);
}

/*
 * Bytes-related primitives
 */

int int_from_hex_char(char ch) {
    if (('0' <= ch) && (ch <= '9')) {
        return ch - '0';
    }
    if (('a' <= ch) && (ch <= 'f')) {
        return ch - 'a' + 10;
    }
    if (('A' <= ch) && (ch <= 'F')) {
        return ch - 'A' + 10;
    }
    throw std::runtime_error("Not a hex character");
}

static Variable fromhex(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string str_without_spaces;
    std::string res;
    for (size_t i = 0; i < str.size(); ++i) {
        if ((str[i] != ' ') && (str[i] != '\t')) {
            str_without_spaces += str[i];
        }
    }
    if (str_without_spaces.size() % 2) {
        throw std::runtime_error("fromhex: even number of chars expected");
    }
    for (size_t i = 0 ; i < str_without_spaces.size(); i += 2) {
        char ch1 = str_without_spaces[i];
        char ch2 = str_without_spaces[i+1];
        res += (16 * int_from_hex_char(ch1) + int_from_hex_char(ch2));
    }
    return encode_string(res);
}

static char to_hex_char(int num) {
    if ((0 <= num) && (num <= 9)) {
        return '0' + num;
    }
    if ((10 <= num) && (num <= 15)) {
        return 'a' + num - 10;
    }
    throw std::runtime_error("Outside single-digit hex range");
}

static Variable hex(const InstructionParams& params, Scope *scope) {
    std::string str = DECODE_STRING(0);
    std::string res;
    for (size_t i = 0; i < str.size(); ++i) {
        res += to_hex_char(str[i] >> 4);
        res += to_hex_char(str[i] % 16);
    }
    return encode_string(res);
}

/*
 * Standard Variable API
 */

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
    // Technically, %-interpolation requires either a single value or a tuple.
    // A list is interpreted as a single value.
    // But since tuples are implemented as lists here, we break the compatibility with Python.
    if (other->get_type() == VariableType::LIST) {
        return NEW_STRING(PercentFormatter(to_str()).format(VAR_TO_LIST(other)));
    }
    else {
        std::vector<Variable> vec;
        vec.push_back(other);
        return NEW_STRING(PercentFormatter(to_str()).format(vec));
    }
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

ListType StringVariable::to_list() {
    ListType res;
    for (size_t i = 0; i < value.size(); ++i) {
        res.push_back(encode_string(std::string(1, value[i])));
    }
    return res;
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
