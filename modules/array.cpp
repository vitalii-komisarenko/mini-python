#include "Module.h"
#include "FunctionParamatersParsing.h"
#include "Instruction.h"
#include "Utils.h"

namespace MiniPython {

Variable array_constructor(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"typecode", "initializer"},
        {
            {"initializer", NEW_LIST()},
        }
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    return std::make_shared<ArrayVariable>(parsed_params.vars["typecode"], VAR_TO_LIST(parsed_params.vars["initializer"]));
}

array::array() {
    SET_FUNCTION("array", array_constructor);
}

} // namespace MiniPython
