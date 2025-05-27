#include "CallContext.h"

namespace MiniPython {

CallContext::CallContext(const InstructionParams &_params, Scope *_scope)
    : params(_params), scope(_scope)
    {}

void CallContext::parse_parameters(const FunctionParameterSchema &schema) {
    parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);
}

} // namespace MiniPython
