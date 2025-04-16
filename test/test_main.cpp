#include <gtest/gtest.h>

extern void test_variable();
extern void test_scope();

int main(int argc, char **argv) {
    test_variable();
    test_scope();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
