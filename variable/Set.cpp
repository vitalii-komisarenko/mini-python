#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

SetVariable::SetVariable() : list() {}

SetVariable::SetVariable(ListType *_list): list(*_list) {}
SetVariable::SetVariable(IterableVariable *_list): list(_list->to_list()) {}

VariableType SetVariable::get_type() {
    return VariableType::SET;
}

ListType SetVariable::get_value() {
    return list;
}

Variable SetVariable::add(const Variable &other) {
    throw std::runtime_error("Addition not supported for sets");
}

Variable SetVariable::sub(const Variable &other) {
    ListType res;
    for (auto &item_this : list) {
        bool found = false;
        for (auto &item_other : list) {
            if (item_this->equal(item_other)) {
                found = true;
                break;
            }
        }
        if (!found) {
            res.push_back(item_this);
        }
    }
    return NEW_SET(&res);
}

Variable SetVariable::mul(const Variable &other) {
    throw std::runtime_error("Multiplication not supported for sets");
}

Variable SetVariable::div(const Variable &other) {
    throw std::runtime_error("Division not supported for sets");
}

Variable SetVariable::mod(const Variable &other) {
    throw std::runtime_error("% not supported for sets");
}

Variable SetVariable::pow(const Variable &other) {
    throw std::runtime_error("Power not supported for sets");
}

Variable SetVariable::int_div(const Variable &other) {
    throw std::runtime_error("Integer division not supported for sets");
}

bool SetVariable::to_bool() {
    return list.size();
}

std::string SetVariable::to_str() {
    std::string delim = ", ";
    std::string result = "{";

    for (size_t i = 0; i < list.size(); ++i) {
        if (i != 0) {
            result += delim;
        }
        // TODO: string: \t, \n, \\ etc.
        result += list[i]->to_str();
    }
    result += "}";

    return result;
}

ListType SetVariable::to_list() {
    return list;
}

bool SetVariable::equal(const Variable &other) {
    auto this_var = NEW_SET(&list);
    return !less(other) && !other->less(this_var);
}

bool SetVariable::less(const Variable &other) {
    for (auto &item_this : list) {
        bool found = false;
        for (auto &item_other : list) {
            if (item_this->equal(item_other)) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool SetVariable::strictly_equal(const Variable &other) {
    return equal(other) && (get_type() == other->get_type());
}

} // namespace MiniPython
