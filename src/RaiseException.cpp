#include "RaiseException.h"

#include <stdexcept>

void raise_exception(const std::string &exception_name, const std::string &description) {
    throw std::runtime_error(exception_name + ": " + description);
}
