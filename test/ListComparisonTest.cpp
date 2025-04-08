#include "variable/Variable.h"

#include <gtest/gtest.h>

using namespace MiniPython;

class ListComparisonTest: public testing::Test {
};

TEST_F(ListComparisonTest, list_comparison) {
    auto empty_list_1 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>());
    auto empty_list_2 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>());
    std::vector<std::shared_ptr<GenericVariable>> vec_1 = {std::static_pointer_cast<GenericVariable>(std::make_shared<IntVariable>(1))};
    std::vector<std::shared_ptr<GenericVariable>> vec_2 = {std::static_pointer_cast<GenericVariable>(std::make_shared<IntVariable>(1))};
    std::vector<std::shared_ptr<GenericVariable>> vec_float = {std::static_pointer_cast<GenericVariable>(std::make_shared<FloatVariable>(1.0))};
    auto list_containing_one_1 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_1));
    auto list_containing_one_2 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_2));
    auto list_containing_one_float = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_float));
    // comparing with itself
    EXPECT_TRUE(empty_list_1->equal(empty_list_1));
    EXPECT_FALSE(empty_list_1->less(empty_list_1));
    EXPECT_TRUE(list_containing_one_1->equal(list_containing_one_1));
    EXPECT_FALSE(list_containing_one_1->less(list_containing_one_1));
    // comparing [] and []
    EXPECT_TRUE(empty_list_1->equal(empty_list_2));
    EXPECT_FALSE(empty_list_1->less(empty_list_2));
    // comparing [] and [1]
    EXPECT_FALSE(empty_list_1->equal(list_containing_one_1));
    EXPECT_FALSE(list_containing_one_1->equal(empty_list_1));
    EXPECT_TRUE(empty_list_1->less(list_containing_one_1));
    EXPECT_FALSE(list_containing_one_1->less(empty_list_1));
    // comparing [1] and [1]
    EXPECT_TRUE(list_containing_one_1->equal(list_containing_one_2));
    EXPECT_FALSE(list_containing_one_1->less(list_containing_one_2));

    std::vector<std::shared_ptr<GenericVariable>> vec_ab = {
        std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>("a")),
        std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>("b")),
    };
    std::vector<std::shared_ptr<GenericVariable>> vec_abc = {
        std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>("a")),
        std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>("b")),
        std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>("c")),
    };
    auto list_ab = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_ab));
    auto list_abc = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_abc));
    // comparing ['a', 'b'] and ['a', 'b', 'c']
    EXPECT_FALSE(list_ab->equal(list_abc));
    EXPECT_FALSE(list_abc->equal(list_ab));
    EXPECT_TRUE(list_ab->less(list_abc));
    EXPECT_FALSE(list_abc->less(list_ab));
}
