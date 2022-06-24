#pragma once

#include "variable/Variable.h"

namespace MiniPython {

class Instruction {
public:
    Variable execute();
};

} // namespace MiniPython
