#pragma once

#include "../variable/Variable.h"

namespace MiniPython {

class os {
public:
    std::shared_ptr<DictVariable> env;
    Variable getenv(Variable key, Variable default_value = NONE);
    Variable putenv(Variable key, Variable value);
    Variable unsetenv(Variable key);
    Variable getcwd();
    Variable chdir(Variable path);
    Variable listdir(Variable path = NEW_STRING("."));
private:
    void make_environ();
};

} // namespace MiniPython
