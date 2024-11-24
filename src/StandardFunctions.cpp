#include "StandardFunctions.h"
#include "Scope.h"
#include "RaiseException.h"
#include "../variable/Variable.h"

#include <iostream>
#include <stdexcept>

#define VAR(i) execute_instruction(params[i], scope)
#define STRING(i) std::dynamic_pointer_cast<StringVariable>(VAR(i))
#define ITERABLE(i) std::dynamic_pointer_cast<IterableVariable>(VAR(i))

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

Variable pow(const InstructionParams &params, Scope *scope) {
    auto arg1 = params[0]->execute(scope);
    auto arg2 = params[1]->execute(scope);

    if (params.size() == 2) {
        return arg1->pow(arg2);
    }

    return arg1->pow(arg2)->mod(params[2]->execute(scope));
}

Variable bool_func(const InstructionParams &params, Scope *scope) {
    bool value = (params.size() > 0) && params[0]->execute(scope)->to_bool();
    return std::make_shared<BoolVariable>(value);
}

std::string _hex(int num) {
    if (num == 0) {
        return "0x0";
    }

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

Variable getattr(const InstructionParams &params, Scope *scope) {
    auto obj = VAR(0);
    auto attr_name = STRING(1)->value;
    return params.size() > 2 ? VAR(2) : obj->get_attr(attr_name);
}

Variable setattr(const InstructionParams &params, Scope *scope) {
    auto obj = VAR(0);
    auto attr_name = STRING(1)->value;
    auto new_value = VAR(2);
    obj->set_attr(attr_name, new_value);
    return None;
}

Variable hasattr(const InstructionParams &params, Scope *scope) {
    auto obj = VAR(0);
    auto attr_name = STRING(1)->value;
    return std::make_shared<BoolVariable>(obj->has_attr(attr_name));
}

Variable list(const InstructionParams &params, Scope *scope) {
    if (params.size()) {
        return std::make_shared<ListVariable>(ITERABLE(0)->to_list());
    }
    return std::make_shared<ListVariable>();
}

Variable set(const InstructionParams &params, Scope *scope) {
    if (params.size()) {
        auto list = ITERABLE(0)->to_list();
        return std::make_shared<SetVariable>(&list);
    }
    return std::make_shared<SetVariable>();
}

Variable input(const InstructionParams &params, Scope *scope) {
    if (params.size()) {
        std::cout << STRING(0)->value;
    }
    std::string line;
    std::getline(std::cin, line);
    return std::make_shared<StringVariable>(line);
}

Variable eval(const InstructionParams &params, Scope *scope) {
    if (!params.size()) {
        raise_exception("TypeError", "eval expected at least 1 argument, got 0");
        return None;
    }

    auto str_var = params[0]->execute(scope);

    if (str_var->get_type() != VariableType::STRING) {
        raise_exception("TypeError", "eval() arg 1 must be a string, bytes or code object");
        return None;
    }

    auto str = str_var->to_str();
    bool str_is_a_var_name = str.size();
    for (char ch: str) {
        if (!isalnum(ch) && (ch != '_')) {
            str_is_a_var_name = false;
        }
    }

    if (str_is_a_var_name) {
        return scope->getVariable(str);
    }

    raise_exception("NotImplementedError", "advanced eval() statements not implemented");
    return None;
}

} // namespace MiniPython::StandardFunctions
