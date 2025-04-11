#include "LineLevelParser.h"
#include "Test.h"

#include <iostream>
#include <gtest/gtest.h>

#define RUN_TEST(function) \
    if (!function()) { \
        std::cerr << #function << " failed\n"; \
    }

using namespace MiniPython;

class ReplaceAllTest: public testing::Test {
};

TEST_F(ReplaceAllTest, empty_str) {
    EXPECT_EQ(replace_all("", "a", "b"), "");
}

TEST_F(ReplaceAllTest, same_size) {
    EXPECT_EQ(replace_all("abcd", "a", "b"), "bbcd");
}

TEST_F(ReplaceAllTest, different_size_1) {
    EXPECT_EQ(replace_all("abcd", "a", "bcd"), "bcdbcd");
}

TEST_F(ReplaceAllTest, different_size_2) {
    EXPECT_EQ(replace_all("abcd", "abc", "e"), "ed");
}

TEST_F(ReplaceAllTest, replace_at_end) {
    EXPECT_EQ(replace_all("abcd", "bcd", "e"), "ae");
}

TEST_F(ReplaceAllTest, replace_full_str) {
    EXPECT_EQ(replace_all("abcd", "abcd", "e"), "e");
}

TEST_F(ReplaceAllTest, multiple_matches) {
    EXPECT_EQ(replace_all("abcdaeae", "ae", "f"), "abcdff");
}

TEST_F(ReplaceAllTest, overlapping_matches) {
    EXPECT_EQ(replace_all("aaaaaaa", "aa", "f"), "fffa");
}

TEST_F(ReplaceAllTest, ignore_accidently_created_matches) {
    EXPECT_EQ(replace_all("aaaaaaa", "aa", "a"), "aaaa");
}

bool test_stringToLines_no_newline_at_the_end() {
    std::string data = "aaa\nbbb\nccc";

    Lines expected = {"aaa", "bbb", "ccc"};
    Lines actual   = stringToLines(data);

    return expected == actual;
}

bool test_stringToLines_has_newline_at_the_end() {
    std::string data = "aaa\nbbb\nccc\n";

    Lines expected = {"aaa", "bbb", "ccc", ""};
    Lines actual   = stringToLines(data);

    return expected == actual;
}

bool test_lineContinuation_dangling_backslash_at_the_end() {
    std::string data = 1 + R"(
aaa
bbb\
ccc
ddd\)";
    Lines expected = {"aaa", "bbbccc", "ddd"};
    Lines actual   = processLineContinuation(stringToLines(data));

    return expected == actual;
}

bool test_lineContinuation_no_dangling_backslash_at_the_end() {
    std::string data = 1 + R"(
aaa
bbb\
ccc
ddd)";
    Lines expected = {"aaa", "bbbccc", "ddd"};
    Lines actual   = processLineContinuation(stringToLines(data));

    return expected == actual;
}

bool test_lineHasCode_empty() {
    return !lineHasCode("");
}

bool test_lineHasCode_space_only() {
    return !lineHasCode(" ");
}

bool test_lineHasCode_tab_only() {
    return !lineHasCode("\t");
}

bool test_lineHasCode_comment_at_the_beginning() {
    return !lineHasCode("# blah blah blah");
}

bool test_lineHasCode_comment_after_whitespace() {
    return !lineHasCode("   # comment");
}

bool test_lineHasCode_code_at_the_beginning() {
    return lineHasCode("a = b + c");
}

bool test_lineHasCode_code_after_whitespace() {
    return lineHasCode("   \t  a = None");
}

bool test_lineHasCode_comment_after_code() {
    return lineHasCode("  x = y # comment");
}

bool test_removeLinesWithoutCode_no_code() {
    Lines lines = {};

    Lines expected = {};
    Lines actual   = removeLinesWithoutCode(lines);

    return expected == actual;
}

bool test_removeLinesWithoutCode_comment_at_the_beginning() {
    Lines lines = {
        "# this functions adds two numbers",
        "def add(a, b):",
        "    retrun a + b",
    };

    Lines expected = {
        "def add(a, b):",
        "    retrun a + b",
    };

    Lines actual = removeLinesWithoutCode(lines);

    return expected == actual;
}

bool test_removeLinesWithoutCode_trailing_empty_line() {
    Lines lines = {
        "def add(a, b):",
        "    retrun a + b",
        "",
    };

    Lines expected = {
        "def add(a, b):",
        "    retrun a + b",
    };

    Lines actual = removeLinesWithoutCode(lines);

    return expected == actual;
}

bool test_removeLinesWithoutCode_mixed_example() {
    Lines lines = {
        "",
        "import math",
        ""
        "import sys",
        "",
        "",
        "    # some comment",
        "print(math.sqrt(16)) # comment",
        "for i in range(30):",
        "    print(i)",
        "",
    };

    Lines expected = {
        "import math",
        "import sys",
        "print(math.sqrt(16)) # comment",
        "for i in range(30):",
        "    print(i)",
    };

    Lines actual = removeLinesWithoutCode(lines);

    return expected == actual;
}

static bool test_linetree_empty() {
    Lines lines = {};
    LineTree lineTree(lines);

    return lineTree.children.size() == 0;
}

static bool test_linetree_one_line_no_indent() {
    Lines lines = {"a = b + c"};
    LineTree lineTree(lines);

    MY_ASSERT_EQUAL(lineTree.children.size(), 1);
    MY_ASSERT_EQUAL(lineTree.children[0]->indentation, "");
    MY_ASSERT_EQUAL(lineTree.children[0]->value, "a = b + c");
    MY_ASSERT_EQUAL(lineTree.children[0]->children.size(), 0);

    return true;
}

static bool test_linetree_tree_line_no_indent() {
    Lines lines = {"a = b + c",
                   "print('Hello')",
                   "a += 2"};
    LineTree lineTree(lines);

    MY_ASSERT_EQUAL(lineTree.children.size(), 3);

    MY_ASSERT_EQUAL(lineTree.children[0]->indentation, "");
    MY_ASSERT_EQUAL(lineTree.children[0]->value, "a = b + c");
    MY_ASSERT_EQUAL(lineTree.children[0]->children.size(), 0);

    MY_ASSERT_EQUAL(lineTree.children[1]->indentation, "");
    MY_ASSERT_EQUAL(lineTree.children[1]->value, "print('Hello')");
    MY_ASSERT_EQUAL(lineTree.children[1]->children.size(), 0);


    MY_ASSERT_EQUAL(lineTree.children[2]->indentation, "");
    MY_ASSERT_EQUAL(lineTree.children[2]->value, "a += 2");
    MY_ASSERT_EQUAL(lineTree.children[2]->children.size(), 0);

    return true;
}

static bool test_linetree_simple_indent() {
    LineTree lineTree{Lines({"if a == b:",
                       "    print('yes')"})};

    MY_ASSERT_EQUAL(lineTree.children.size(), 1);

    MY_ASSERT_EQUAL(lineTree.children[0]->indentation, "");
    MY_ASSERT_EQUAL(lineTree.children[0]->value, "if a == b:");
    MY_ASSERT_EQUAL(lineTree.children[0]->children.size(), 1);

    MY_ASSERT_EQUAL(lineTree.children[0]->children[0]->indentation, "    ");
    MY_ASSERT_EQUAL(lineTree.children[0]->children[0]->value, "print('yes')");
    MY_ASSERT_EQUAL(lineTree.children[0]->children[0]->children.size(), 0);

    return true;
}

static bool test_linetree_big_tree() {
    Lines lines = {
        "print ('blah')",
        "if x == 3:",
        "    pass",
        "i = 5",
        "while i > 0:",
        "\tprint(i)",
        "\tif i < 3:",
        "\t print('i is less than 3')",
        "\t i = i - 0.02",
        "i=4",
    };
    LineTree lineTree(lines);

    MY_ASSERT_EQUAL(lineTree.children.size(), 5);
    MY_ASSERT_EQUAL(lineTree.children[0]->children.size(), 0);
    MY_ASSERT_EQUAL(lineTree.children[1]->children.size(), 1);
    MY_ASSERT_EQUAL(lineTree.children[2]->children.size(), 0);
    MY_ASSERT_EQUAL(lineTree.children[3]->children.size(), 2);
    MY_ASSERT_EQUAL(lineTree.children[4]->children.size(), 0);

    MY_ASSERT_EQUAL(lineTree.children[1]->children[0]->children.size(), 0);

    MY_ASSERT_EQUAL(lineTree.children[3]->children[0]->children.size(), 0);
    MY_ASSERT_EQUAL(lineTree.children[3]->children[1]->children.size(), 2);

    return true;
}

void test_line_level_parser() {
    // stringToLines
    RUN_TEST(test_stringToLines_no_newline_at_the_end);
    RUN_TEST(test_stringToLines_has_newline_at_the_end);

    // processLineContinuation
    RUN_TEST(test_lineContinuation_dangling_backslash_at_the_end);
    RUN_TEST(test_lineContinuation_no_dangling_backslash_at_the_end);

    // lineHasCode
    RUN_TEST(test_lineHasCode_empty);
    RUN_TEST(test_lineHasCode_space_only);
    RUN_TEST(test_lineHasCode_tab_only);
    RUN_TEST(test_lineHasCode_comment_at_the_beginning);
    RUN_TEST(test_lineHasCode_comment_after_whitespace);
    RUN_TEST(test_lineHasCode_code_at_the_beginning);
    RUN_TEST(test_lineHasCode_code_after_whitespace);
    RUN_TEST(test_lineHasCode_comment_after_code);

    // removeLinesWithoutCode
    RUN_TEST(test_removeLinesWithoutCode_no_code);
    RUN_TEST(test_removeLinesWithoutCode_comment_at_the_beginning);
    RUN_TEST(test_removeLinesWithoutCode_trailing_empty_line);
    RUN_TEST(test_removeLinesWithoutCode_mixed_example);

    // LineTree
    RUN_TEST(test_linetree_empty);
    RUN_TEST(test_linetree_one_line_no_indent);
    RUN_TEST(test_linetree_tree_line_no_indent);
    RUN_TEST(test_linetree_simple_indent);
    RUN_TEST(test_linetree_big_tree);
}
