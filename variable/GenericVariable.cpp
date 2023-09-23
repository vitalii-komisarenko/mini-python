#include "Variable.h"

namespace MiniPython {

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
