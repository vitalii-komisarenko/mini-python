#include <gtest/gtest.h>

extern void test_variable();
extern void test_scope();
extern void test_line_level_parser();

int main(int argc, char **argv) {
    test_variable();
    test_scope();
    test_line_level_parser();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
