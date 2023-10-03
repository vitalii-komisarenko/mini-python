#pragma once

#include "../variable/Variable.h"

namespace MiniPython {

class time {
public:
    Variable time_func();
    Variable time_ns();
    Variable sleep(Variable seconds_var);
};

} // namespace MiniPython
