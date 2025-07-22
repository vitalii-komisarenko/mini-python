#include "Module.h"
#include "Instruction.h"
#include "RaiseException.h"

#include <chrono>
#include <thread>

namespace MiniPython {

static Variable time_func(const InstructionParams &params, Scope *scope) {
    auto milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1);
    return NEW_FLOAT(milliseconds_since_epoch);
}

static Variable time_ns(const InstructionParams &params, Scope *scope) {
    auto nanoseconds_since_start = std::chrono::system_clock::now().time_since_epoch() / std::chrono::nanoseconds(1);
    return NEW_INT(nanoseconds_since_start);
}

static Variable sleep(const InstructionParams &params, Scope *scope) {
    auto seconds_var = PARAM(0);

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
        raise_exception("TypeError", "time.sleep: a number expected");
    }

    long nanoseconds = 1000 * 1000 * 1000 * seconds;
    std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    return NONE;
}

time::time() {
    SET_FUNCTION("time", time_func);
    SET_FUNCTION("time_ns", time_ns);
    SET_FUNCTION("sleep", sleep);
}

} // namespace MiniPython
