#include "LineLevelParser.h"

#include <gtest/gtest.h>

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

class StringToLinesTest: public testing::Test {
};

TEST_F(StringToLinesTest, no_newline_at_the_end) {
    EXPECT_EQ(stringToLines("aaa\nbbb\nccc"), Lines({"aaa", "bbb", "ccc"}));
}

TEST_F(StringToLinesTest, newline_at_the_end) {
    EXPECT_EQ(stringToLines("aaa\nbbb\nccc\n"), Lines({"aaa", "bbb", "ccc", ""}));

}

class ProcessLineContinuationTest: public testing::Test {
};

TEST_F(ProcessLineContinuationTest, dangling_backslash_at_the_end) {
    std::string data = 1 + R"(
aaa
bbb\
ccc
ddd\)";
    EXPECT_ANY_THROW(processLineContinuation(stringToLines(data)));
}

TEST_F(ProcessLineContinuationTest, no_dangling_backslash_at_the_end) {
    std::string data = 1 + R"(
aaa
bbb\
ccc
ddd)";
    EXPECT_EQ(processLineContinuation(stringToLines(data)), Lines({"aaa", "bbbccc", "ddd"}));
}

class LineHasCodeTest: public testing::Test {
};

TEST_F(LineHasCodeTest, empty) {
    EXPECT_FALSE(lineHasCode(""));
}

TEST_F(LineHasCodeTest, space_only) {
    EXPECT_FALSE(lineHasCode(" "));
}

TEST_F(LineHasCodeTest, tab_only) {
    EXPECT_FALSE(lineHasCode("\t"));
}

TEST_F(LineHasCodeTest, comment_at_the_beginning) {
    EXPECT_FALSE(lineHasCode("# blah blah blah"));
}

TEST_F(LineHasCodeTest, comment_after_whitespace) {
    EXPECT_FALSE(lineHasCode("   # comment"));
}

TEST_F(LineHasCodeTest, code_at_the_beginning) {
    EXPECT_TRUE(lineHasCode("a = b + c"));
}

TEST_F(LineHasCodeTest, code_after_whitespace) {
    EXPECT_TRUE(lineHasCode("   \t  a = None"));
}

TEST_F(LineHasCodeTest, comment_after_code) {
    EXPECT_TRUE(lineHasCode("  x = y # comment"));
}

class RemoveLinesWithoutCodeTest: public testing::Test {
};

TEST_F(RemoveLinesWithoutCodeTest, no_code) {
    EXPECT_EQ(removeLinesWithoutCode({}), Lines({}));
}

TEST_F(RemoveLinesWithoutCodeTest, comment_at_the_beginning) {
    Lines lines = {
        "# this functions adds two numbers",
        "def add(a, b):",
        "    retrun a + b",
    };

    Lines expected = {
        "def add(a, b):",
        "    retrun a + b",
    };

    EXPECT_EQ(removeLinesWithoutCode(lines), expected);
}

TEST_F(RemoveLinesWithoutCodeTest, trailing_empty_line) {
    Lines lines = {
        "def add(a, b):",
        "    retrun a + b",
        "",
    };

    Lines expected = {
        "def add(a, b):",
        "    retrun a + b",
    };

    EXPECT_EQ(removeLinesWithoutCode(lines), expected);
}

TEST_F(RemoveLinesWithoutCodeTest, mixed_example) {
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

    EXPECT_EQ(removeLinesWithoutCode(lines), expected);
}

class LineTreeTest: public testing::Test {
};

TEST_F(LineTreeTest, empty) {
    Lines lines = {};
    LineTree lineTree(lines);

    EXPECT_EQ(lineTree.children.size(), 0);
}

TEST_F(LineTreeTest, one_line_no_indent) {
    Lines lines = {"a = b + c"};
    LineTree lineTree(lines);

    EXPECT_EQ(lineTree.children.size(), 1);
    EXPECT_EQ(lineTree.children[0]->indentation, "");
    EXPECT_EQ(lineTree.children[0]->value, "a = b + c");
    EXPECT_EQ(lineTree.children[0]->children.size(), 0);
}

TEST_F(LineTreeTest, three_lines_no_indent) {
    Lines lines = {"a = b + c",
                   "print('Hello')",
                   "a += 2"};
    LineTree lineTree(lines);

    EXPECT_EQ(lineTree.children.size(), 3);

    EXPECT_EQ(lineTree.children[0]->indentation, "");
    EXPECT_EQ(lineTree.children[0]->value, "a = b + c");
    EXPECT_EQ(lineTree.children[0]->children.size(), 0);

    EXPECT_EQ(lineTree.children[1]->indentation, "");
    EXPECT_EQ(lineTree.children[1]->value, "print('Hello')");
    EXPECT_EQ(lineTree.children[1]->children.size(), 0);


    EXPECT_EQ(lineTree.children[2]->indentation, "");
    EXPECT_EQ(lineTree.children[2]->value, "a += 2");
    EXPECT_EQ(lineTree.children[2]->children.size(), 0);
}

TEST_F(LineTreeTest, simple_indent) {
    LineTree lineTree{Lines({"if a == b:",
                       "    print('yes')"})};

    EXPECT_EQ(lineTree.children.size(), 1);

    EXPECT_EQ(lineTree.children[0]->indentation, "");
    EXPECT_EQ(lineTree.children[0]->value, "if a == b:");
    EXPECT_EQ(lineTree.children[0]->children.size(), 1);

    EXPECT_EQ(lineTree.children[0]->children[0]->indentation, "    ");
    EXPECT_EQ(lineTree.children[0]->children[0]->value, "print('yes')");
    EXPECT_EQ(lineTree.children[0]->children[0]->children.size(), 0);
}

TEST_F(LineTreeTest, big_tree) {
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

    EXPECT_EQ(lineTree.children.size(), 5);
    EXPECT_EQ(lineTree.children[0]->children.size(), 0);
    EXPECT_EQ(lineTree.children[1]->children.size(), 1);
    EXPECT_EQ(lineTree.children[2]->children.size(), 0);
    EXPECT_EQ(lineTree.children[3]->children.size(), 2);
    EXPECT_EQ(lineTree.children[4]->children.size(), 0);

    EXPECT_EQ(lineTree.children[1]->children[0]->children.size(), 0);

    EXPECT_EQ(lineTree.children[3]->children[0]->children.size(), 0);
    EXPECT_EQ(lineTree.children[3]->children[1]->children.size(), 2);
}
