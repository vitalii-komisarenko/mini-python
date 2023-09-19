#include "variable/Variable.h"
#include "Instruction.h"

#include <functional>
#include <memory>

namespace MiniPython::StandardFunctions {

Variable print(const InstructionParams &params, Scope *scope);

Variable min(const InstructionParams &params, Scope *scope);
Variable max(const InstructionParams &params, Scope *scope);

Variable all(const InstructionParams &params, Scope *scope);
Variable any(const InstructionParams &params, Scope *scope);

} // namespace MiniPython::StandardFunctions
