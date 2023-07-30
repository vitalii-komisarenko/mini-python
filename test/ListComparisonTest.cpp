#include "ListComparisonTest.h"
#include "variable/Variable.h"
#include "Test.h"

using namespace MiniPython;

void test_list_comparison() {
    auto empty_list_1 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>());
    auto empty_list_2 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>());
    std::vector<std::shared_ptr<GenericVariable>> vec_1 = {std::static_pointer_cast<GenericVariable>(std::make_shared<IntVariable>(1))};
    std::vector<std::shared_ptr<GenericVariable>> vec_2 = {std::static_pointer_cast<GenericVariable>(std::make_shared<IntVariable>(1))};
    std::vector<std::shared_ptr<GenericVariable>> vec_float = {std::static_pointer_cast<GenericVariable>(std::make_shared<FloatVariable>(1.0))};
    auto list_containing_one_1 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_1));
    auto list_containing_one_2 = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_2));
    auto list_containing_one_float = std::static_pointer_cast<GenericVariable>(std::make_shared<ListVariable>(vec_float));
    // comparing with itself
    MY_ASSERT(empty_list_1->equal(empty_list_1));
    MY_ASSERT(!empty_list_1->less(empty_list_1));
    MY_ASSERT(list_containing_one_1->equal(list_containing_one_1));
    MY_ASSERT(!list_containing_one_1->less(list_containing_one_1));
    // comparing [] and []
    MY_ASSERT(empty_list_1->equal(empty_list_2));
    MY_ASSERT(!empty_list_1->less(empty_list_2));
    // comparing [] and [1]
    MY_ASSERT(!empty_list_1->equal(list_containing_one_1));
    MY_ASSERT(!list_containing_one_1->equal(empty_list_1));
    MY_ASSERT(empty_list_1->less(list_containing_one_1));
    MY_ASSERT(!list_containing_one_1->less(empty_list_1));
    // comparing [1] and [1]
    MY_ASSERT(list_containing_one_1->equal(list_containing_one_2));
    MY_ASSERT(!list_containing_one_1->less(list_containing_one_2));

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
    MY_ASSERT(!list_ab->equal(list_abc));
    MY_ASSERT(!list_abc->equal(list_ab));
    MY_ASSERT(list_ab->less(list_abc));
    MY_ASSERT(!list_abc->less(list_ab));
}
