#pragma once

#include "variable/Variable.h"

#define PARAM(i) (params[i]->execute(scope))
#define PARAM_DEFAULT(i, DEFAULT_VALUE) \
        ((i < params.size()) ? (PARAM(i)) : (DEFAULT_VALUE))

#define SET_FUNCTION(PYTHON_FUNCTION_NAME, CPP_FUNCTION_NAME) \
        set_attr(PYTHON_FUNCTION_NAME, std::make_shared<FunctionVariable>(CPP_FUNCTION_NAME))

namespace MiniPython {

class array: public ModuleVariable {
public:
    array();
};

class base64: public ModuleVariable {
public:
    base64();
};

class binascii: public ModuleVariable {
public:
    binascii();

    static std::string helper_hexlify(const std::string &data, const std::string &sep, int bytes_per_sep);
    static std::string base64_encode(const std::string &input);
    static std::string base64_decode(const std::string &input);
};

class ipaddress: public ModuleVariable {
public:
    ipaddress();
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
