#pragma once

#include "variable/Variable.h"

#define PARAM(i) params[i]->execute(scope)
#define PARAM_DEFAULT(i, DEFAULT_VALUE) \
        (params.size() < i) ? PARAM(i) : DEFAULT_VALUE;

#define SET_FUNCTION(PYTHON_FUNCTION_NAME, CPP_FUNCTION_NAME) \
        set_attr(PYTHON_FUNCTION_NAME, std::make_shared<FunctionVariable>(CPP_FUNCTION_NAME))

namespace MiniPython {

class binascii: public ModuleVariable {
public:
    binascii();
};

class math: public ModuleVariable {
public:
    math();
};

class os: public ModuleVariable {
public:
    os();
private:
    std::shared_ptr<DictVariable> env;
    void make_environ();
};

class sys: public ModuleVariable {
public:
    sys();
};

class time: public ModuleVariable {
public:
    time();
};

} // namespace MiniPython
