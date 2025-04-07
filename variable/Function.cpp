#include "Variable.h"
#include "Instruction.h"

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

Variable FunctionVariable::call(const InstructionParams &params, Scope *scope) {
    return value(params, scope);
}

Variable FunctionVariable::call(Variable &param, Scope *scope) {
    InstructionParams params;
    params.push_back(std::make_shared<Instruction>(param));
    return value(params, scope);
}

Variable FunctionVariable::call(Variable &param1, Variable &param2, Scope *scope) {
    InstructionParams params;
    params.push_back(std::make_shared<Instruction>(param1));
    params.push_back(std::make_shared<Instruction>(param2));
    return value(params, scope);
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
