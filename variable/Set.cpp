#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

extern Variable execute_instruction(std::shared_ptr<Instruction> instr, Scope *scope);

#define VAR(i) execute_instruction(params[i], scope)
#define ITERABLE(i) std::dynamic_pointer_cast<IterableVariable>(VAR(i))
#define SET(i) std::dynamic_pointer_cast<SetVariable>(VAR(i))

static bool set_contains(SetVariable *set, Variable item) {
    for (auto set_item : set->list) {
        if (set_item->equal(item)) {
            return true;
        }
    }
    return false;
}

static Variable contains(const InstructionParams& params, Scope *scope) {
    return set_contains(SET(0).get(), VAR(1)) ? TRUE : FALSE;
}

static Variable add(const InstructionParams& params, Scope *scope) {
    if (!contains(params, scope)) {
        SET(0)->list.push_back(VAR(1));
    }
    return NONE;
}

static Variable update(const InstructionParams& params, Scope *scope) {
    auto set = SET(0);
    for (size_t i = 1; i < params.size(); ++i) {
        for (auto &item: ITERABLE(i)->to_list()) {
            if (!set_contains(set.get(), item)) {
                set->list.push_back(item);
            }
        }
    }
    return NONE;
}

static bool _discard(const InstructionParams& params, Scope *scope) {
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


static Variable discard(const InstructionParams& params, Scope *scope) {
    _discard(params, scope);
    return NONE;
}

static Variable remove(const InstructionParams& params, Scope *scope) {
    if (!_discard(params, scope)) {
        throw std::runtime_error("KeyError");
    }
    return NONE;
}

static Variable pop(const InstructionParams& params, Scope *scope) {
    auto set = SET(0)->list;
    if (!set.size()) {
        throw std::runtime_error("KeyError: 'pop from an empty set'");
    }
    auto res = set[set.size() - 1];
    set.pop_back();
    return res;
}

static Variable clear(const InstructionParams& params, Scope *scope) {
    SET(0)->list.clear();
    return NONE;
}

static Variable issuperset(const InstructionParams& params, Scope *scope) {
    return SET(0)->less(ITERABLE(1)) ? FALSE : TRUE;
}

static Variable issubset(const InstructionParams& params, Scope *scope) {
    return (SET(0)->less(ITERABLE(1)) || SET(0)->equal(ITERABLE(1))) ? TRUE : FALSE;
}

static Variable isdisjoint(const InstructionParams& params, Scope *scope) {
    auto set1 = SET(0);
    auto set2 = SET(1);
    return (set1->sub(set2)->to_bool() || set2->sub(set1)->to_bool()) ? FALSE : TRUE;
}

static Variable copy(const InstructionParams& params, Scope *scope) {
    auto list = SET(0)->to_list();
    return NEW_SET(&list);
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
