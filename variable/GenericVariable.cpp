#include "Variable.h"

namespace MiniPython {

Variable GenericVariable::get_attr(const std::string &name) {
    return attr[name];
}

Variable GenericVariable::set_attr(const std::string &name, Variable attr_value) {
    attr[name] = attr_value;
}

} // namespace MiniPython
