#include "variable/Variable.h"

#include "Test.h"

#define VAR(TYPE, VALUE) \
    std::static_pointer_cast<GenericVariable>(std::make_shared<TYPE ## Variable>(VALUE))

#define CHECK_VAR(_VAR, TYPE1, TYPE2, VALUE)                                 \
    MY_ASSERT(_VAR->get_type()  == VariableType::TYPE1);                     \
    if (_VAR->get_type()  == VariableType::TYPE1) {                          \
        auto converted = std::dynamic_pointer_cast<TYPE2 ## Variable>(_VAR); \
        MY_ASSERT(converted->get_value() == VALUE);                          \
    }

void test_variable() {
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

    auto FloatZero = VAR(Float, 0);
    auto FloatOne  = VAR(Float, 1);
    auto FloatTwo  = VAR(Float, 2);
    auto FloatTwo2 = VAR(Float, 2);

    MY_ASSERT(!None->equal(FloatZero));
    MY_ASSERT(!FloatZero->equal(None));

    MY_ASSERT(False->equal(FloatZero));
    MY_ASSERT(FloatZero->equal(False));
    MY_ASSERT(!False->equal(FloatOne));
    MY_ASSERT(!FloatOne->equal(False));
    MY_ASSERT(!False->equal(FloatTwo));
    MY_ASSERT(!FloatTwo->equal(False));

    MY_ASSERT(!True->equal(FloatZero));
    MY_ASSERT(!FloatZero->equal(True));
    MY_ASSERT(True->equal(FloatOne));
    MY_ASSERT(FloatOne->equal(True));
    MY_ASSERT(!True->equal(FloatTwo));
    MY_ASSERT(!FloatTwo->equal(True));

    MY_ASSERT(!False->less(FloatZero));
    MY_ASSERT(!FloatZero->less(False));
    MY_ASSERT(False->less(FloatOne));
    MY_ASSERT(!FloatOne->less(False));
    MY_ASSERT(False->less(FloatTwo));
    MY_ASSERT(!FloatTwo->less(False));

    MY_ASSERT(!True->less(FloatZero));
    MY_ASSERT(FloatZero->less(True));
    MY_ASSERT(!True->less(FloatOne));
    MY_ASSERT(!FloatOne->less(True));
    MY_ASSERT(True->less(FloatTwo));
    MY_ASSERT(!FloatTwo->less(True));

    MY_ASSERT(!FloatZero->equal(FloatOne));
    MY_ASSERT(FloatTwo->equal(FloatTwo));
    MY_ASSERT(FloatTwo->equal(FloatTwo2));

    MY_ASSERT(FloatZero->less(FloatOne));
    MY_ASSERT(!FloatOne->less(FloatZero));
    MY_ASSERT(!FloatTwo->less(FloatTwo2));

    MY_ASSERT(IntZero->equal(FloatZero));
    MY_ASSERT(FloatZero->equal(IntZero));

    MY_ASSERT(!IntZero->less(FloatZero));
    MY_ASSERT(!FloatZero->less(IntZero));

    MY_ASSERT(!IntOne->less(FloatZero));
    MY_ASSERT(!FloatOne->less(IntZero));

    MY_ASSERT(IntZero->less(FloatOne));
    MY_ASSERT(FloatZero->less(IntOne));

    MUST_THROW(None->add(None));
    MUST_THROW(None->add(None2));
    MUST_THROW(None->add(False));
    MUST_THROW(None->add(IntZero));
    MUST_THROW(None->add(FloatTwo));

    MUST_THROW(None->sub(None));
    MUST_THROW(None->sub(None2));
    MUST_THROW(None->sub(False));
    MUST_THROW(None->sub(IntZero));
    MUST_THROW(None->sub(FloatTwo));

    MUST_THROW(None->mul(None));
    MUST_THROW(None->mul(None2));
    MUST_THROW(None->mul(False));
    MUST_THROW(None->mul(IntZero));
    MUST_THROW(None->mul(FloatTwo));

    MUST_THROW(None->div(None));
    MUST_THROW(None->div(None2));
    MUST_THROW(None->div(False));
    MUST_THROW(None->div(IntZero));
    MUST_THROW(None->div(FloatTwo));

    MUST_THROW(None->mod(None));
    MUST_THROW(None->mod(None2));
    MUST_THROW(None->mod(False));
    MUST_THROW(None->mod(IntZero));
    MUST_THROW(None->mod(FloatTwo));

    MUST_THROW(None->pow(None));
    MUST_THROW(None->pow(None2));
    MUST_THROW(None->pow(False));
    MUST_THROW(None->pow(IntZero));
    MUST_THROW(None->pow(FloatTwo));

    MY_ASSERT(None->to_bool() == false);
    MY_ASSERT(None->to_str() == "None");

    MUST_THROW(False->add(None));
    CHECK_VAR(False->add(False), INT, Int, 0);
    CHECK_VAR(False->add(False2), INT, Int, 0);
    CHECK_VAR(False->add(True), INT, Int, 1);
    CHECK_VAR(False->add(IntTwo), INT, Int, 2);
    CHECK_VAR(False->add(FloatOne), FLOAT, Float, 1);

    MUST_THROW(False->sub(None));
    CHECK_VAR(False->sub(False), INT, Int, 0);
    CHECK_VAR(False->sub(False2), INT, Int, 0);
    CHECK_VAR(False->sub(True), INT, Int, -1);
    CHECK_VAR(False->sub(IntTwo), INT, Int, -2);
    CHECK_VAR(False->sub(FloatOne), FLOAT, Float, -1);
}
