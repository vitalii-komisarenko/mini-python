#include "Variable.h"

#include <stdexcept>

namespace MiniPython {

FunctionVariable::FunctionVariable(FunctionType& function)
    : value(function)
    {}

VariableType FunctionVariable::get_type() {
    return VariableType::FUNCTION;
}

const FunctionType& FunctionVariable::get_value() {
    return value;
}

Variable FunctionVariable::call(const InstructionParams &params) {
    return value(params);
}

#define OP_NOT_SUPPORTED(OPERATION)\
Variable FunctionVariable::OPERATION(const Variable &other) { \
    throw std::runtime_error("Operation '" #OPERATION "' not supported on functions"); \
}

OP_NOT_SUPPORTED(add);
OP_NOT_SUPPORTED(sub);
OP_NOT_SUPPORTED(mul);
OP_NOT_SUPPORTED(div);
OP_NOT_SUPPORTED(int_div);
OP_NOT_SUPPORTED(mod);
OP_NOT_SUPPORTED(pow);

bool FunctionVariable::to_bool() {
    return true;
}

std::string FunctionVariable::to_str() {
    // Python3 returns something like:
    //'<built-in function abs>'
    //'<built-in function print>'
    // but mini-python does not store function names
    return "<function>";
}

bool FunctionVariable::equal(const Variable &other) {
    return strictly_equal(other);
}

bool FunctionVariable::less(const Variable &other) {
    throw std::runtime_error("Can't use < or > with functions");
}

bool FunctionVariable::strictly_equal(const Variable &other) {
    if (get_type() != other->get_type()) {
        return false;
    }

    return get_value() == std::dynamic_pointer_cast<FunctionVariable>(other)->get_value();
}

} // namespace MiniPython