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
        {VariableType::BYTES, "bytes"},
        {VariableType::LIST, "list"},
        {VariableType::ARRAY, "array"},
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

IntType GenericVariable::to_int() {
    throw std::runtime_error(get_class_name() + " cannot be converted to int");
}

Variable GenericVariable::to_int_var() {
    return NEW_INT(to_int());
}

std::string GenericVariable::to_str() {
    return "<object>";
}

Variable GenericVariable::to_bytes_variable() {
    throw std::runtime_error(get_class_name() + " can't be converted to bytes");
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
    throw std::runtime_error("get_attr() should not be called from GenericVariable");
}

void GenericVariable::set_attr(const std::string &name, Variable attr_value) {
    throw std::runtime_error("set_attr() should not be called from GenericVariable");
}

bool GenericVariable::has_attr(const std::string &name) {
    throw std::runtime_error("has_attr() should not be called from GenericVariable");
}

} // namespace MiniPython
