#include "Module.h"
#include "Instruction.h"
#include "FunctionParamatersParsing.h"

namespace MiniPython {

Variable v4_int_to_packed(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"address"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    int int_value = parsed_params.vars["address"]->to_int();

    std::string result;
    result += (int_value >> 24) & 0xFF;
    result += (int_value >> 16) & 0xFF;
    result += (int_value >> 8) & 0xFF;
    result += int_value & 0xFF;

    return NEW_BYTES(result);
}

ipaddress::ipaddress() {
    SET_FUNCTION("v4_int_to_packed", v4_int_to_packed);
}

} // namespace MiniPython
