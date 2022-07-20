#include "variable/Variable.h"
#include "Instruction.h"

#include <functional>
#include <memory>

namespace MiniPython::StandardFunctions {

Variable print(const InstructionParams &params, std::shared_ptr<Scope> scope);

} // namespace MiniPython::StandardFunctions
