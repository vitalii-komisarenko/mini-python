#include "Module.h"
#include "Instruction.h"

#include <cstdlib>

namespace MiniPython {

static Variable exit(const InstructionParams &params, Scope *scope) {
    auto exit_code = PARAM_DEFAULT(0, NEW_INT(0));
    ::exit(VAR_TO_INT(exit_code));
    return NONE;
}

sys::sys() {
    SET_FUNCTION("exit", exit);
}

} // namespace MiniPython
