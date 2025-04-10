#include "Variable.h"

namespace MiniPython {

std::string GenericVariable::get_class_name() {
    static std::unordered_map<VariableType, const char*> mapping = {
        {VariableType::NONE, "NoneType"},
        {VariableType::INT, "int"},
        {VariableType::BOOL, "bool"},
        {VariableType::FLOAT, "float"},
        {VariableType::COMPLEX, "complex"},
        {VariableType::STRING, "str"},
        {VariableType::LIST, "list"},
        {VariableType::SET, "set"},
        {VariableType::DICT, "dict"},
        {VariableType::FUNCTION, "function"},
        {VariableType::MODULE, "module"},
    };

    return mapping.find(get_type()) == mapping.end()
         ? mapping[get_type()]
         : "type";
}

Variable GenericVariable::add(const Variable &other) {
    throw std::runtime_error("Operation + not supported");
}

Variable GenericVariable::sub(const Variable &other) {
    throw std::runtime_error("Operation - not supported");
}

Variable GenericVariable::mul(const Variable &other) {
    throw std::runtime_error("Operation * not supported");
}

Variable GenericVariable::div(const Variable &other) {
    throw std::runtime_error("Operation / not supported");
}

Variable GenericVariable::int_div(const Variable &other) {
    throw std::runtime_error("Operation // not supported");
}

Variable GenericVariable::mod(const Variable &other) {
    throw std::runtime_error("Operation % not supported");
}

Variable GenericVariable::pow(const Variable &other) {
    throw std::runtime_error("Operation ** not supported");
}

bool GenericVariable::to_bool() {
    return true;
}

std::string GenericVariable::to_str() {
    return "<object>";
}

bool GenericVariable::equal(const Variable &other) {
    throw std::runtime_error("Operation == not supported");
}

bool GenericVariable::less(const Variable &other) {
    throw std::runtime_error("Operation < not supported");
}

bool GenericVariable::strictly_equal(const Variable &other) {
    throw std::runtime_error("Operation `strictly_equal` not supported");
}

Variable GenericVariable::get_attr(const std::string &name) {
    return attr[name];
}

void GenericVariable::set_attr(const std::string &name, Variable attr_value) {
    attr[name] = attr_value;
}

bool GenericVariable::has_attr(const std::string &name) {
    return attr.find(name) != attr.end();
}

} // namespace MiniPython
