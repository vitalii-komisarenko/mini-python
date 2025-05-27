#pragma once

#include "FunctionParamatersParsing.h"
#include "Instruction.h"
#include "Scope.h"
#include "Variable.h"

namespace MiniPython {

class CallContext {
public:
    CallContext(const InstructionParams &_params, Scope *_scope);
    void parse_parameters(const FunctionParameterSchema &schema);
    Variable self;
    Variable get_var(const char *var_name);
private:
    const InstructionParams &params;
    Scope *scope;
    ParsedFunctionParamaters parsed_params;
};

} // namespace MiniPython
