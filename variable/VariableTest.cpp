#include "Variable.h"

#include <iostream>

#define MY_ASSERT(X)                                                       \
    if (!(X)) {                                                                \
        std::cerr << __FILE__ << " " << __LINE__ << ": " << #X << " failed\n"; \
    }

#define VAR(TYPE, VALUE) \
    std::static_pointer_cast<GenericVariable>(std::make_shared<TYPE ## Variable>(VALUE))


using namespace MiniPython;

int main() {
    auto None  = std::static_pointer_cast<GenericVariable>(std::make_shared<NoneVariable>());
    auto None2 = std::static_pointer_cast<GenericVariable>(std::make_shared<NoneVariable>());

    MY_ASSERT(None->equal(None));
    MY_ASSERT(None->equal(None2));

    auto False  = VAR(Bool, false);
    auto False2 = VAR(Bool, false);
    auto True   = VAR(Bool, true);
    auto True2  = VAR(Bool, true);

    MY_ASSERT(!None->equal(False));
    MY_ASSERT(!None->equal(True));
    MY_ASSERT(!False->equal(None));
    MY_ASSERT(!True->equal(None));

    MY_ASSERT(False->equal(False));
    MY_ASSERT(False->equal(False2));

    MY_ASSERT(True->equal(True));
    MY_ASSERT(True->equal(True2));

    MY_ASSERT(!True->equal(False));
    MY_ASSERT(!False->equal(True));

    MY_ASSERT(!False->less(False));
    MY_ASSERT(False->less(True));
    MY_ASSERT(!True->less(False));
    MY_ASSERT(!True->less(True));

    auto IntZero = VAR(Int, 0);
    auto IntOne  = VAR(Int, 1);
    auto IntTwo  = VAR(Int, 2);
    auto IntTwo2 = VAR(Int, 2);

    MY_ASSERT(!None->equal(IntZero));
    MY_ASSERT(!IntZero->equal(None));

    MY_ASSERT(False->equal(IntZero));
    MY_ASSERT(IntZero->equal(False));
    MY_ASSERT(!False->equal(IntOne));
    MY_ASSERT(!IntOne->equal(False));
    MY_ASSERT(!False->equal(IntTwo));
    MY_ASSERT(!IntTwo->equal(False));

    MY_ASSERT(!True->equal(IntZero));
    MY_ASSERT(!IntZero->equal(True));
    MY_ASSERT(True->equal(IntOne));
    MY_ASSERT(IntOne->equal(True));
    MY_ASSERT(!True->equal(IntTwo));
    MY_ASSERT(!IntTwo->equal(True));

    MY_ASSERT(!False->less(IntZero));
    MY_ASSERT(!IntZero->less(False));
    MY_ASSERT(False->less(IntOne));
    MY_ASSERT(!IntOne->less(False));
    MY_ASSERT(False->less(IntTwo));
    MY_ASSERT(!IntTwo->less(False));

    MY_ASSERT(!True->less(IntZero));
    MY_ASSERT(IntZero->less(True));
    MY_ASSERT(!True->less(IntOne));
    MY_ASSERT(!IntOne->less(True));
    MY_ASSERT(True->less(IntTwo));
    MY_ASSERT(!IntTwo->less(True));

    MY_ASSERT(!IntZero->equal(IntOne));
    MY_ASSERT(IntTwo->equal(IntTwo));
    MY_ASSERT(IntTwo->equal(IntTwo2));

    MY_ASSERT(IntZero->less(IntOne));
    MY_ASSERT(!IntOne->less(IntZero));
    MY_ASSERT(!IntTwo->less(IntTwo2));
}
