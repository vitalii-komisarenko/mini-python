#pragma once

#include "../variable/Variable.h"

namespace MiniPython {

class time {
public:
    Variable time_func();
    Variable sleep(Variable seconds_var);
};

} // namespace MiniPython
