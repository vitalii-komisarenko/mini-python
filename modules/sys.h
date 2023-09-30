#pragma once

#include "../variable/Variable.h"

namespace MiniPython {

class sys {
public:
    Variable exit(Variable exit_code);
};

} // namespace MiniPython
