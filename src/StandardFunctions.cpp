#include "StandardFunctions.h"

#include <iostream>
#include <stdexcept>

namespace MiniPython::StandardFunctions {

static auto None = std::static_pointer_cast<GenericVariable>(std::make_shared<NoneVariable>());

Variable print(const InstructionParams &params, Scope *scope) {
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << params[i]->execute(scope)->to_str();
        bool is_last = i == params.size() - 1;
        std::cout << (is_last ? "\n" : " ");
    }
    return None;
}

Variable min(const InstructionParams &params, Scope *scope) {
    if (params.size() == 0) {
        throw std::runtime_error("TypeError: min expected at least 1 argument, got 0");
    }

    auto res = std::static_pointer_cast<GenericVariable>(params[0]->execute(scope));

    for (size_t i = 0; i < params.size(); ++i) {
        auto var = params[i]->execute(scope);
        if (var->less(res)) {
            res = var;
        }
    }

    return res;
}

Variable max(const InstructionParams &params, Scope *scope) {
    if (params.size() == 0) {
        throw std::runtime_error("TypeError: min expected at least 1 argument, got 0");
    }

    auto res = std::static_pointer_cast<GenericVariable>(params[0]->execute(scope));

    for (size_t i = 0; i < params.size(); ++i) {
        auto var = params[i]->execute(scope);
        if (!var->less(res)) {
            res = var;
        }
    }

    return res;
}

Variable bool_func(const InstructionParams &params, Scope *scope) {
    bool value = (params.size() > 0) && params[0]->execute(scope)->to_bool();
    return std::make_shared<BoolVariable>(value);
}

std::string _hex(int num) {
    if (num < 0) {
        return "-" + _hex(-num);
    }

    std::string res;
    while (num > 0) {
        static std::string digits = "0123456789abcdef";
        int new_digit = num % 16;
        num /= 16;
        res = std::string(1, digits[new_digit]) + res;
    }

    return "0x" + res;
}

Variable hex(const InstructionParams &params, Scope *scope) {
    int num = std::dynamic_pointer_cast<IntVariable>(params[0]->execute(scope))->value;
    return std::make_shared<StringVariable>(_hex(num));
}

Variable ord(const InstructionParams &params, Scope *scope) {
    auto generic_var = params[0]->execute(scope);
    unsigned char ch = std::dynamic_pointer_cast<StringVariable>(generic_var)->value[0];
    auto int_var = std::make_shared<IntVariable>(ch);
    return std::dynamic_pointer_cast<GenericVariable>(int_var);
}

Variable len(const InstructionParams &params, Scope *scope) {
    if (params[0]->execute(scope)->get_type() == VariableType::STRING) {
        auto generic_var = params[0]->execute(scope);
        size_t value = std::dynamic_pointer_cast<StringVariable>(generic_var)->value.size();
        auto int_var = std::make_shared<IntVariable>(value);
        return std::dynamic_pointer_cast<GenericVariable>(int_var);
    }

    throw std::runtime_error("Unsupported type for len");
}

} // namespace MiniPython::StandardFunctions
