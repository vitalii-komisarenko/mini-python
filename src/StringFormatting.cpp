#include "StringFormatting.h"

#include <cctype>
#include <sstream>

namespace MiniPython {

int get_number_from_stringstream(std::stringstream &ss) {
    std::string num_str;
    char ch;
    while ((ch = ss.get()) != EOF) {
        if (!isdigit(ch)) {
            ss.putback(ch);
            break;
        }
        num_str += ch;
    }
    return std::atoi(num_str.c_str());
}

ParsedFormat::ParsedFormat(const std::string &format): plus(false), dot(false), octothorp(false), after_dot(0), after_octothorp(0) {
    std::stringstream ss(format);
    char ch;
    after_octothorp = 0;
    while ((ch = ss.get()) != EOF) {
        if (ch == '+') {
            plus = true;
        }
        else if (isdigit(ch)) {
            ss.putback(ch);
        }
        else if (ch == '.') {
            dot = true;
            after_dot = get_number_from_stringstream(ss);
        }
        else if (ch == '#') {
            octothorp = true;
            after_octothorp = get_number_from_stringstream(ss);
        }
        else if (isalpha(ch)) {
            letter += ch;
        }
    }
}

FormatElement::FormatElement(FormatElementType _type, const std::string &_value): type(_type), value(_value) {}

enum class PercentFormatterState {
    READING_EXACT_STRING,
    READING_FORMAT,
};

PercentFormatter::PercentFormatter(const std::string &format) {
    PercentFormatterState state = PercentFormatterState::READING_EXACT_STRING;
    std::stringstream ss(format);
    char ch;
    std::string value;
    while((ch = ss.get()) != EOF) {
        if (state == PercentFormatterState::READING_EXACT_STRING) {
            if (ch == '%') {
                elements.push_back(FormatElement(FormatElementType::EXACT_STRING, value));
                value = ch;
                state = PercentFormatterState::READING_FORMAT;
            }
            else {
                value += ch;
            }
        }
        if (state == PercentFormatterState::READING_FORMAT) {
            value += ch;
            if (isalpha(ch)) {
                elements.push_back(FormatElement(FormatElementType::INTERPOLATION, value));
                value = "";
                state = PercentFormatterState::READING_EXACT_STRING;
            }
        }
    }

    auto last_elem_type = (state == PercentFormatterState::READING_EXACT_STRING)
                        ? FormatElementType::EXACT_STRING
                        : FormatElementType::INTERPOLATION;
    elements.push_back(FormatElement(last_elem_type, value));
}

std::string interpolate_value(const std::string &format, Variable var) {
    ParsedFormat f(format);
    std::string str;
    if (f.letter == "s") {
        str = var->to_str();
        if ((f.dot) && (str.length() > f.after_dot)) {
            str = str.substr(0, f.after_dot);
        }
    }
    else if (f.letter == "d") {
        str = std::to_string(std::stoi(var->to_str()));
    }
    else if (f.letter == "f") {
        str = std::to_string(std::stod(var->to_str()));
        if (f.dot) {
            size_t pos = str.find(".");
            if (pos == std::string::npos) {
                str += "." + std::string(pos, '0');
            }
            else {
                str += std::string(str.length() - pos, '0');
            }
        }
    }

    if (str.length() < f.after_octothorp) {
        str = std::string(f.after_octothorp - str.length(), ' ') + str;
    }

    return str;
}

std::string PercentFormatter::format(const std::vector<Variable> &variables) {
    std::string res;
    size_t var_idx = 0;
    for (const auto& elem : elements) {
        if (elem.type == FormatElementType::EXACT_STRING) {
            res += elem.value;
        }
        else {
            if (var_idx >= variables.size()) {
                throw std::runtime_error("percent formatting: not enough variables");
            }
            res += interpolate_value(elem.value, variables[var_idx++]);
        }
    }
    return res;
}

}; // namespace MiniPython
