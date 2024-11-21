#include "Variable.h"

#include <algorithm>
#include <stdexcept>

namespace MiniPython {

extern Variable execute_instruction(std::shared_ptr<Instruction> instr, Scope *scope);

#define VAR(i) execute_instruction(params[i], scope)
#define INT(i) std::dynamic_pointer_cast<IntVariable>(VAR(i))
#define LIST(i) std::dynamic_pointer_cast<ListVariable>(VAR(i))

#define NEW_LIST(vector) std::make_shared<ListVariable>(vector)

Variable append(const InstructionParams& params, Scope *scope) {
    LIST(0)->list.push_back(VAR(1));
    return NONE;
}

Variable insert(const InstructionParams& params, Scope *scope) {
    LIST(0)->list.insert(LIST(0)->list.begin() + INT(1)->value, VAR(2));
    return NONE;
}

Variable clear(const InstructionParams& params, Scope *scope) {
    LIST(0)->list.clear();
    return NONE;
}

Variable reverse(const InstructionParams& params, Scope *scope) {
    auto vec = LIST(0)->list;
    std::reverse(vec.begin(), vec.end());
    return NONE;
}

Variable copy(const InstructionParams& params, Scope *scope) {
    return NEW_LIST(LIST(0)->list);
}

ListVariable::ListVariable() {}
ListVariable::ListVariable(ListType _list): list(_list) {}

VariableType ListVariable::get_type() {
    return VariableType::LIST;
}

ListType ListVariable::get_value() {
    return list;
}

Variable ListVariable::add(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::LIST: {
        ListType result = this->list;
        auto other_casted = std::dynamic_pointer_cast<ListVariable>(other);
        result.insert(result.end(), other_casted->list.begin(), other_casted->list.end());
        return std::make_shared<ListVariable>(result);
    }
    default:
        throw std::runtime_error("Can't add this to list");
    }
}

Variable ListVariable::sub(const Variable &other) {
    throw std::runtime_error("Can't substract anything from list");
}

Variable ListVariable::mul(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::INT: {
        auto other_casted = std::dynamic_pointer_cast<IntVariable>(other);

        ListType result;
        for (IntVariable::IntType i = 0; i < other_casted->get_value(); ++i) {
            result.insert(result.end(), this->list.begin(), this->list.end());
        }
        return std::make_shared<ListVariable>(result);
    }
    case VariableType::BOOL: {
        auto other_casted = std::dynamic_pointer_cast<BoolVariable>(other);
        return mul(other_casted->toIntVar());
    }
    default:
        throw std::runtime_error("Can't multiply list by that");
    }
}

Variable ListVariable::div(const Variable &other) {
    throw std::runtime_error("Can't divide list by anything");
}

Variable ListVariable::int_div(const Variable &other) {
    throw std::runtime_error("Can't divide list by anything");
}

Variable ListVariable::mod(const Variable &other) {
    throw std::runtime_error("Can't do modular arithmetic on list");
}

Variable ListVariable::pow(const Variable &other) {
    throw std::runtime_error("Can't raise list to any power");
}

bool ListVariable::to_bool() {
    return list.size() != 0;
}

std::string ListVariable::to_str() {
    std::string delim = ", ";
    std::string result = "[";

    for (size_t i = 0; i < list.size(); ++i) {
        if (i != 0) {
            result += delim;
        }
        // TODO: string: \t, \n, \\ etc.
        result += list[i]->to_str();
    }
    result += "]";

    return result;
}

ListType ListVariable::to_list() {
    return list;
}

bool ListVariable::equal(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::LIST: {
        auto other_casted = std::dynamic_pointer_cast<ListVariable>(other);
        if (this->list.size() != other_casted->list.size()) {
            return false;
        }
        for (size_t i = 0; i < list.size(); ++i) {
            if (!list[i]->equal(other_casted->list[i])) {
                return false;
            }
        }
        return true;
    }
    default:
        return false;
    }
}

bool ListVariable::less(const Variable &other) {
    switch (other->get_type()) {
    case VariableType::LIST: {
        auto other_casted = std::dynamic_pointer_cast<ListVariable>(other);
        size_t max_size = this->list.size() > other_casted->list.size()
                        ? this->list.size()
                        : other_casted->list.size();
        for (size_t i = 0; i < max_size; ++ i) {
            if (i >= this->list.size()) {
                return true;
            }
            if (i >= other_casted->list.size()) {
                return false;
            }
            if (this->list[i]->less(other_casted->list[i])) {
                return true;
            }
            if (other_casted->list[i]->less(this->list[i])) {
                return false;
            }
        }
        return false;
    }
    default:
        throw std::runtime_error("Can't use < and > with list an non-list");
    }
}

bool ListVariable::strictly_equal(const Variable &other) {
    if (get_type() != other->get_type()) {
        return false;
    }

    return list == std::dynamic_pointer_cast<ListVariable>(other)->list;
}

} // namespace MiniPython
