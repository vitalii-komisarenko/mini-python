#pragma once

#include <stdexcept>

namespace MiniPython {

class Exception: public std::exception {
public:
    Exception(const std::string &_description);
    virtual std::string name();
    std::string what();
private:
    std::string description;
};

class ZeroDivisionError: public Exception {
public:
    ZeroDivisionError(const std::string &_description);
    std::string name() override;
private:
    std::string description;
};

} // namespace MiniPython
