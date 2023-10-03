#include "time.h"

#include <chrono>
#include <thread>

namespace MiniPython {

Variable time::time_func() {
    auto milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1);
    return NEW_FLOAT(milliseconds_since_epoch);
}

Variable time::time_ns() {
    auto nanoseconds_since_start = std::chrono::system_clock::now().time_since_epoch() / std::chrono::nanoseconds(1);
    return NEW_INT(nanoseconds_since_start);
}

Variable time::sleep(Variable seconds_var) {
    float seconds;
    if (seconds_var->get_type() == VariableType::INT) {
        seconds = VAR_TO_INT(seconds_var);
    }
    else if (seconds_var->get_type() == VariableType::FLOAT) {
        seconds = VAR_TO_FLOAT(seconds_var);
    }
    else if (seconds_var->get_type() == VariableType::BOOL) {
        seconds = VAR_TO_BOOL(seconds_var) ? 1 : 0;
    }
    else {
        RAISE("TypeError", "time.sleep: a number expected");
    }

    long nanoseconds = 1000 * 1000 * 1000 * seconds;
    std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    return NONE;
}

} // namespace MiniPython
