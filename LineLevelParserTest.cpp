#include "LineLevelParser.h"

#include <iostream>

#define RUN_TEST(function) \
    if (!function()) { \
        std::cerr << #function << " failed\n"; \
    }

using namespace MiniPython;


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

int main() {
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
    return 0;
}
