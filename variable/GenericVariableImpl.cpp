#include "Variable.h"

namespace MiniPython {

Variable GenericVariableImpl::get_attr(const std::string &name) {
    return attr[name];
}

void GenericVariableImpl::set_attr(const std::string &name, Variable attr_value) {
    attr[name] = attr_value;
}

bool GenericVariableImpl::has_attr(const std::string &name) {
    return attr.find(name) != attr.end();
}

};
