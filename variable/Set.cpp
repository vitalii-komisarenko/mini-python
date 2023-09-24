#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

extern Variable execute_instruction(std::shared_ptr<Instruction> instr, Scope *scope);

#define VAR(i) execute_instruction(params[i], scope)
#define ITERABLE(i) std::dynamic_pointer_cast<IterableVariable>(VAR(i))
#define SET(i) std::dynamic_pointer_cast<SetVariable>(VAR(i))

Variable contains(const InstructionParams& params, Scope *scope) {
    auto item = VAR(1);
    for (auto &item_this : SET(0)->list) {
        if (item_this->equal(item)) {
            return true;
        }
    }
    return false;
}

Variable add(const InstructionParams& params, Scope *scope) {
    if (!contains(params)) {
        SET(0)->list.push_back(VAR(1));
    }
    return NONE;
}

Variable update(const InstructionParams& params, Scope *scope) {
    auto set = SET(0);
    for (size_t i = 1; i < params.size(); ++i) {
        for (auto &item: ITERABLE(i)->to_list()) {
            auto new_params = std::make_shared<vector<Variable>>();
            new_params.push_back(set);
            new_params.push_back(item);
            add(new_params, scope);
        }
    }
    return NONE;
}

bool _discard(const InstructionParams& params, Scope *scope) {
    auto item = VAR(1);
    auto set = SET(0)->list;
    for (size_t i = 0; i < set.size(); ++i) {
        if (set[i]->equal(item)) {
            set.erase(set.begin() + i);
            return true;
        }
    }
    return false;
}


Variable discard(const InstructionParams& params, Scope *scope) {
    _discard(params, scope);
}

Variable remove(const InstructionParams& params, Scope *scope) {
    if (!_discard(params, scope)) {
        throw std::runtime_error("KeyError");
    }
}

Variable pop(const InstructionParams& params, Scope *scope) {
    auto set = SET(0)->list;
    if (!set.size()) {
        throw std::runtime_error("KeyError: 'pop from an empty set'");
    }
    auto res = set[set.size() - 1];
    set.pop_back();
    return res;
}

Variable clear(const InstructionParams& params, Scope *scope) {
    SET(0)->list.clear();
}

Variable issuperset(const InstructionParams& params, Scope *scope) {
    return !SET(0)->less(ITERABLE(1));
}

Variable issubset(const InstructionParams& params, Scope *scope) {
    return SET(0)->less(ITERABLE(1)) || SET(0)->equal(ITERABLE(1));
}

Variable isdisjoint(const InstructionParams& params, Scope *scope) {
    auto set1 = SET(0);
    auto set2 = SET(1);
    return !(set1->sub(set2)->to_bool() || set2->sub(set1)->to_bool());
}

Variable copy(const InstructionParams& params, Scope *scope) {
    return NEW_SET(&(SET(0)->list()));
}

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
