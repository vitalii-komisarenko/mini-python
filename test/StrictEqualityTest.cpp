#include "variable/Variable.h"

#include <gtest/gtest.h>

#include <vector>

using namespace MiniPython;

class StringEqualityTest: public testing::Test {
};

TEST_F(StringEqualityTest, strict_equality) {
    auto int_1 = NEW_INT(1);
    EXPECT_TRUE(int_1->strictly_equal(int_1));
    EXPECT_TRUE(int_1->strictly_equal(NEW_INT(1)));
    EXPECT_FALSE(int_1->strictly_equal(NEW_FLOAT(1.0)));
    EXPECT_FALSE(int_1->strictly_equal(NEW_BOOL(true)));

    std::vector<Variable> vars = {
        NEW_INT(0),
        NEW_FLOAT(0.0),
        NEW_BOOL(false),
        NEW_STRING(""),
        std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>())
    };

    for (size_t i = 0; i < vars.size(); ++i) {
        for (size_t j = 0; j < vars.size(); ++j) {
            EXPECT_EQ(vars[i]->strictly_equal(vars[j]), i == j);
        }
    }
}
