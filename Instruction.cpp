#include "Instruction.h"

namespace MiniPython {

Variable Instruction::execute() {
    return std::make_shared<NoneVariable>();
}

} // namespace MiniPython
