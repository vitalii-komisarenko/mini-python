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
    };

    return mapping.find(get_type()) == mapping.end()
         ? mapping[get_type()]
         : "type";
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
