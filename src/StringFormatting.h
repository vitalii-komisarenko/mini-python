#pragma once

#include "variable/Variable.h"

namespace MiniPython {

struct ParsedFormat {
    bool plus;
    bool dot;
    bool octothorp;
    int after_dot;
    int after_octothorp;
    std::string letter;
    ParsedFormat(const std::string &format);
};

enum class FormatElementType {
    EXACT_STRING,
    INTERPOLATION,
};

struct FormatElement {
    FormatElementType type;
    std::string value;
    FormatElement(FormatElementType _type, const std::string &_value);
};

class PercentFormatter {
public:
    PercentFormatter(const std::string &format);
    std::string format(const std::vector<Variable> &variables);
private:
    std::vector<FormatElement> elements;
};

class FStringFormatter {
public:
    FStringFormatter(const std::string &format);
    std::string format(Scope *scope);
private:
    std::vector<FormatElement> elements;
};

std::string interpolate_value(const std::string &format, Variable var);

}; // namespace MiniPython
