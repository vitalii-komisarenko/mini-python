#include "sys.h"

#include <cstdlib>

namespace MiniPython {

Variable sys::exit(Variable exit_code) {
    ::exit(VAR_TO_INT(exit_code));
    return NONE;
}

} // namespace MiniPython
