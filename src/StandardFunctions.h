#include "variable/Variable.h"
#include "Instruction.h"

#include <functional>
#include <memory>

namespace MiniPython::StandardFunctions {

Variable print(const InstructionParams &params, Scope *scope);

Variable min(const InstructionParams &params, Scope *scope);
Variable max(const InstructionParams &params, Scope *scope);
Variable pow(const InstructionParams &params, Scope *scope);

Variable bool_func(const InstructionParams &params, Scope *scope);

Variable hex(const InstructionParams &params, Scope *scope);
Variable ord(const InstructionParams &params, Scope *scope);

Variable len(const InstructionParams &params, Scope *scope);

Variable list(const InstructionParams &params, Scope *scope);
Variable set(const InstructionParams &params, Scope *scope);

Variable eval_string(const std::string &str, Scope *scope);
Variable eval(const InstructionParams &params, Scope *scope);

} // namespace MiniPython::StandardFunctions
