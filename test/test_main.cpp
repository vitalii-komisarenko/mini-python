#include "ListComparisonTest.h"

extern void test_variable();
extern void test_token();
extern void test_token2variable();
extern void test_instruction();
extern void test_scope();
extern void test_line_level_parser();

int main() {
    test_variable();
    test_list_comparison();
    test_token();
    test_token2variable();
    test_instruction();
    test_scope();
    test_line_level_parser();
}
