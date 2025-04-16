#include <gtest/gtest.h>

extern void test_variable();

int main(int argc, char **argv) {
    test_variable();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
