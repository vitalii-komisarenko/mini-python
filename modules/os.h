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
    Variable mkdir(Variable path, Variable mode = NEW_INT(0777));
    Variable makedirs(Variable path, Variable mode = NEW_INT(0777), Variable exists_ok = NEW_BOOL(false));
private:
    void make_environ();
};

} // namespace MiniPython
