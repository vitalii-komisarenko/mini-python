#include <gtest/gtest.h>
#include "src/StringFormatting.h"
#include "src/Scope.h"

using namespace MiniPython;

class ParsedFormatTest: public testing::Test {
};

TEST_F(ParsedFormatTest, test_simple_format) {
    ParsedFormat f("%s");
    EXPECT_EQ(f.plus, false);
    EXPECT_EQ(f.dot, false);
    EXPECT_EQ(f.octothorp, false);
    EXPECT_EQ(f.letter, "s");
}

TEST_F(ParsedFormatTest, test_format_with_dot) {
    ParsedFormat f("%.2f");
    EXPECT_EQ(f.plus, false);
    EXPECT_EQ(f.dot, true);
    EXPECT_EQ(f.after_dot, 2);
    EXPECT_EQ(f.octothorp, false);
    EXPECT_EQ(f.letter, "f");
}

TEST_F(ParsedFormatTest, test_format_with_dot_multichar) {
    ParsedFormat f("%.15f");
    EXPECT_EQ(f.plus, false);
    EXPECT_EQ(f.dot, true);
    EXPECT_EQ(f.after_dot, 15);
    EXPECT_EQ(f.octothorp, false);
    EXPECT_EQ(f.letter, "f");
}

TEST_F(ParsedFormatTest, test_format_with_dot_no_char) {
    ParsedFormat f("%.f");
    EXPECT_EQ(f.plus, false);
    EXPECT_EQ(f.dot, true);
    EXPECT_EQ(f.after_dot, 0);
    EXPECT_EQ(f.octothorp, false);
    EXPECT_EQ(f.letter, "f");
}

TEST_F(ParsedFormatTest, test_format_with_octothorp) {
    ParsedFormat f("%#5f");
    EXPECT_EQ(f.plus, false);
    EXPECT_EQ(f.dot, false);
    EXPECT_EQ(f.octothorp, true);
    EXPECT_EQ(f.after_octothorp, 5);
    EXPECT_EQ(f.letter, "f");
}

TEST_F(ParsedFormatTest, test_format_with_dot_and_octothorp) {
    ParsedFormat f("%#13.4f");
    EXPECT_EQ(f.plus, false);
    EXPECT_EQ(f.dot, true);
    EXPECT_EQ(f.after_dot, 4);
    EXPECT_EQ(f.octothorp, true);
    EXPECT_EQ(f.after_octothorp, 13);
    EXPECT_EQ(f.letter, "f");
}

TEST_F(ParsedFormatTest, test_format_with_dot_and_octothorp_and_plus) {
    ParsedFormat f("%+#13.25f");
    EXPECT_EQ(f.plus, true);
    EXPECT_EQ(f.dot, true);
    EXPECT_EQ(f.after_dot, 25);
    EXPECT_EQ(f.octothorp, true);
    EXPECT_EQ(f.after_octothorp, 13);
    EXPECT_EQ(f.letter, "f");
}

TEST_F(ParsedFormatTest, test_format_with_dot_and_octothorp_and_plus2) {
    ParsedFormat f("%+#21.3s");
    EXPECT_EQ(f.plus, true);
    EXPECT_EQ(f.dot, true);
    EXPECT_EQ(f.after_dot, 3);
    EXPECT_EQ(f.octothorp, true);
    EXPECT_EQ(f.after_octothorp, 21);
    EXPECT_EQ(f.letter, "s");
}

class InterpolateStringTest: public testing::Test {
};

TEST_F(InterpolateStringTest, interpolate) {
    EXPECT_EQ(interpolate_value("%s", NEW_STRING("")), "");
    EXPECT_EQ(interpolate_value("%s", NEW_STRING("")), "");
}

TEST_F(InterpolateStringTest, interpolate_vars) {
    EXPECT_EQ(NEW_STRING("%+#21.3s")->mod(NEW_FLOAT(3456.78))->to_str(), "                  345");
    EXPECT_EQ(NEW_STRING("%#21.3sdef")->mod(NEW_FLOAT(3456.78))->to_str(), "                  345def");
    EXPECT_EQ(NEW_STRING("abc%#21.3s")->mod(NEW_FLOAT(3456.78))->to_str(), "abc                  345");
    EXPECT_EQ(NEW_STRING("abc%#21.3sdef")->mod(NEW_FLOAT(3456.78))->to_str(), "abc                  345def");
}

class FStringFormatterTest: public testing::Test {
};

TEST_F(FStringFormatterTest, format) {
    Scope scope;
    scope.setVariable("str", NEW_STRING("abcdef"));
    EXPECT_EQ(FStringFormatter("{str}").format(&scope), "abcdef");
}
