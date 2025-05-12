#include "Module.h"
#include "Instruction.h"

namespace MiniPython {

static Variable do_nothing(const InstructionParams &params, Scope *scope) {
    return NONE;
}

gc::gc() {
    SET_FUNCTION("enable", do_nothing);
    SET_FUNCTION("disable", do_nothing);
}

} // namespace MiniPython
