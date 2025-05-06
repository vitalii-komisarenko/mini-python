#include <string>
#include <unordered_map>

#include "variable/Variable.h"

namespace MiniPython {

struct FunctionParameterSchema {
    std::vector<std::string> param_names;
    std::unordered_map<std::string, Variable> default_values;
    FunctionParameterSchema(const std::vector<std::string> _param_names,
                            const std::unordered_map<std::string, Variable> _default_values);
};

struct ParsedFunctionParamaters {
    std::unordered_map<std::string, Variable> vars;
    std::shared_ptr<ListVariable> args;
    std::shared_ptr<DictVariable> kwargs;

    ParsedFunctionParamaters();

    static ParsedFunctionParamaters parse(const InstructionParams &params,
                                          Scope *scope,
                                          const FunctionParameterSchema &schema);
};

} // namespace MiniPython
