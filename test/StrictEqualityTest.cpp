#include "StrictEqualityTest.h"
#include "variable/Variable.h"
#include "Test.h"

#define VAR(TYPE, VALUE) \
    std::static_pointer_cast<GenericVariable>(std::make_shared<TYPE ## Variable>(VALUE))

void test_strict_equality() {
    std::vector<std::shared_ptr<GenericVariable>> vars = {
        VAR(Int, 1),
        VAR(Int, 1),
        VAR(Float, 1.0),
        VAR(Bool, true),
    };
    MY_ASSERT(vars[0]->strictly_equal(vars[0]));
    MY_ASSERT(vars[0]->strictly_equal(vars[1]));
    MY_ASSERT(!vars[0]->strictly_equal(vars[2]));
    MY_ASSERT(!vars[0]->strictly_equal(vars[3]));

    vars = {
        VAR(Int, 0),
        VAR(Float, 0.0),
        VAR(Bool, false),
        VAR(String, ""),
        std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>())
    };

    for (size_t i = 0; i < vars.size(); ++i) {
        for (size_t j = 0; j < vars.size(); ++j) {
            MY_ASSERT_EQUAL(vars[i]->strictly_equal(vars[j]), i == j);
        }
    }
}
