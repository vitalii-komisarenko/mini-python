#include "FunctionParamatersParsing.h"

#include "Instruction.h"

namespace MiniPython {

FunctionParameterSchema::FunctionParameterSchema(const std::vector<std::string> _param_names,
                                                 const std::unordered_map<std::string, Variable> _default_values):
    param_names(_param_names),
    default_values(_default_values) {}

ParsedFunctionParamaters::ParsedFunctionParamaters():
    args(std::make_shared<ListVariable>()),
    kwargs(std::make_shared<DictVariable>()) {}

ParsedFunctionParamaters ParsedFunctionParamaters::parse(const InstructionParams &params,
                                                         Scope *scope,
                                                         const FunctionParameterSchema &schema) {
    ParsedFunctionParamaters result;

    for (auto pair: schema.default_values) {
        result.vars[pair.first] = pair.second;
    }

    size_t positional_parameter_index = 0;

    for (auto &param: params) {
        if (param->op == Operation::KWARG) {
            auto var_name = param->params[0]->var->to_str();
            auto value = execute_instruction(param->params[1], scope);

            bool is_kwarg = true;
            for (auto &v: schema.param_names) {
                if (v == var_name) {
                    is_kwarg = false;
                    break;
                }
            }

            if (is_kwarg) {
                result.kwargs->set_item(NEW_STRING(var_name), value);
            }
            else {
                result.vars[var_name] = value;
            }
        }
        else {
            auto value = execute_instruction(param, scope);
            if (positional_parameter_index < schema.param_names.size()) {
                auto var_name = schema.param_names[positional_parameter_index++];
                result.vars[var_name] = value;
            }
            else {
                result.args->list.push_back(value);
            }
        }
    }

    return result;
}

} // namespace MiniPython
