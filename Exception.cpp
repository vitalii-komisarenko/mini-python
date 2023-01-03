#include "Exception.h"

#define DEFINE_EXCEPTION(NAME) \
    NAME::NAME(const std::string &_description): \
        description(_description) {}; \
\
    std::string NAME::name() { \
        return #NAME; \
    }

namespace MiniPython {

DEFINE_EXCEPTION(Exception)

std::string Exception::what() {
    return name() + ": " + description;
}

ZeroDivisionError::ZeroDivisionError(const std::string &_description):
    Exception(_description), description(_description) {}

} // namespace MiniPython
