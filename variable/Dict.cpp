#include "Variable.h"

#include <stdexcept>

#define NEW_COMPLEX(real, imag) std::make_shared<ComplexVariable>(real, imag)

namespace MiniPython {

DictVariable::DictVariable() {}

DictVariable::DictVariable(const Variable &keys, const Variable value) {
    for (auto &key : std::dynamic_pointer_cast<IterableVariable>(keys)->to_list()) {
        pairs.push_back({key, value});
    }
}

VariableType DictVariable::get_type() {
    return VariableType::DICT;
}

Variable DictVariable::add(const Variable &other) { throw std::runtime_error("dict: + not supported"); }
Variable DictVariable::sub(const Variable &other) { throw std::runtime_error("dict: - not supported"); }
Variable DictVariable::mul(const Variable &other) { throw std::runtime_error("dict: * not supported"); }
Variable DictVariable::div(const Variable &other) { throw std::runtime_error("dict: / not supported"); }
Variable DictVariable::mod(const Variable &other) { throw std::runtime_error("dict: % not supported"); }
Variable DictVariable::pow(const Variable &other) { throw std::runtime_error("dict: ^ not supported"); }
Variable DictVariable::int_div(const Variable &other) { throw std::runtime_error("dict: // not supported"); }

bool DictVariable::to_bool() {
    return pairs.size();
}

std::string DictVariable::to_str() {
    std::string res = "{";
    for (auto &pair: pairs) {
        res += pair.first->to_str();
        res += ": ";
        res += pair.second->to_str();
        res += ", ";
    }
    return res + "}";
}

ListType DictVariable::keys() {
    ListType res;
    for (auto &pair: pairs) {
        res.push_back(pair.first);
    }
    return res;
}


ListType DictVariable::values() {
    ListType res;
    for (auto &pair: pairs) {
        res.push_back(pair.second);
    }
    return res;
}

ListType DictVariable::to_list() {
    return keys();
}

Variable DictVariable::get_item_helper(Variable key) {
    for (auto &pair: pairs) {
        if (key == pair.first) {
            return pair.second;
        }
    }
    return OBJECT_NOT_FOUND;
}

Variable DictVariable::get_item(Variable key) {
    auto value = get_item_helper(key);
    if (value->equal(OBJECT_NOT_FOUND)) {
        throw std::runtime_error("KeyError");
    }
    return value;
}

Variable DictVariable::get(Variable key, Variable default_value) {
    auto value = get_item_helper(key);
    return value->equal(OBJECT_NOT_FOUND) ? default_value : value;
}

Variable DictVariable::set_item(Variable key, Variable value) {
    for (auto &pair : pairs) {
        if (pair.first->equal(key)) {
            pair.second = value;
            return NONE;
        }
    }
    pairs.push_back({key, value});
    return NONE;
}

Variable DictVariable::clear() {
    pairs.clear();
    return NONE;
}

Variable DictVariable::copy() {
    auto res = std::make_shared<DictVariable>();
    res->pairs = pairs;
    return res;
}

Variable DictVariable::del_item_helper(Variable key) {
    for (size_t i = 0; i < pairs.size(); ++i) {
        if (pairs[i].first->equal(key)) {
            auto res = pairs[i].second;
            pairs.erase(pairs.begin() + i);
            return res;
        }
    }
    return OBJECT_NOT_FOUND;
}

Variable DictVariable::pop(Variable key) {
    auto value = del_item_helper(key);
    if (value->equal(OBJECT_NOT_FOUND)) {
        throw std::runtime_error("KeyError");
    }
    return value;
}

Variable DictVariable::popitem() {
    if (pairs.size() == 0) {
        throw std::runtime_error("KeyError: 'popitem(): dictionary is empty'");
    }
    auto pair = pairs[pairs.size() - 1];
    pairs.pop_back();
    auto res = std::make_shared<ListVariable>();
    res->list.push_back(pair.first);
    res->list.push_back(pair.second);
    return res;
}

bool DictVariable::equal(const Variable &other) {
    auto key_list1 = keys();
    auto key_list2 = std::dynamic_pointer_cast<DictVariable>(other)->keys();
    auto keys1 = NEW_SET(&key_list1);
    auto keys2 = NEW_SET(&key_list2);
    if (!keys1->equal(keys2)) {
        return false;
    }
}

bool DictVariable::less(const Variable &other) {
    throw std::runtime_error("dict: < not supported");
}

bool DictVariable::strictly_equal(const Variable &other) {
    return get_type() == other->get_type() && equal(other);
}

} // namespace MiniPython
