#include "Parser.h"

#include <gtest/gtest.h>

using namespace MiniPython;

class ParserIsPrefixTest: public testing::Test {
};

TEST_F(ParserIsPrefixTest, prefix_in_empty_line) {
    EXPECT_FALSE(Parser("").isPrefix("a"));
}

TEST_F(ParserIsPrefixTest, prefix_in_empty_line_multichar) {
    EXPECT_FALSE(Parser("").isPrefix("abcdef"));
}

TEST_F(ParserIsPrefixTest, exact_match) {
    EXPECT_TRUE(Parser("a").isPrefix("a"));
}

TEST_F(ParserIsPrefixTest, exact_match_multichar) {
    EXPECT_TRUE(Parser("abcdef").isPrefix("abcdef"));
}

TEST_F(ParserIsPrefixTest, wrong_short_prefix) {
    EXPECT_FALSE(Parser("abcdef").isPrefix("xyz"));
}

TEST_F(ParserIsPrefixTest, wrong_long_prefix) {
    EXPECT_FALSE(Parser("xyz").isPrefix("abcdef"));
}

TEST_F(ParserIsPrefixTest, substr_present_but_not_prefix) {
    EXPECT_FALSE(Parser("abcdef").isPrefix("def"));
}

TEST_F(ParserIsPrefixTest, prefix_in_empty_line_not_from_beginning) {
    Parser parser("zzz");
    parser.pos = 3;
    EXPECT_FALSE(parser.isPrefix("a"));
}

TEST_F(ParserIsPrefixTest, prefix_in_empty_line_multichar_not_from_beginning) {
    Parser parser("zzz");
    parser.pos = 3;
    EXPECT_FALSE(parser.isPrefix("abcdef"));
}

TEST_F(ParserIsPrefixTest, exact_match_not_from_beginning) {
    Parser parser("zzza");
    parser.pos = 3;
    EXPECT_TRUE(parser.isPrefix("a"));
}

TEST_F(ParserIsPrefixTest, exact_match_multichar_not_from_beginning) {
    Parser parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_TRUE(parser.isPrefix("abcdef"));
}

TEST_F(ParserIsPrefixTest, wrong_short_prefix_not_from_beginning) {
    Parser parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_FALSE(parser.isPrefix("xyz"));
}

TEST_F(ParserIsPrefixTest, wrong_long_prefix_not_from_beginning) {
    Parser parser("zzzxyz");
    parser.pos = 3;
    EXPECT_FALSE(parser.isPrefix("abcdef"));
}

TEST_F(ParserIsPrefixTest, substr_present_but_not_prefix_not_from_beginning) {
    Parser parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_FALSE(parser.isPrefix("def"));
}

TEST_F(ParserIsPrefixTest, substr_present_already_read) {
    Parser parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_FALSE(parser.isPrefix("zzz"));
}

class ParserIsOneOfPrefixesTest: public testing::Test {
};

TEST_F(ParserIsOneOfPrefixesTest, prefix_in_empty_line) {
    EXPECT_EQ(Parser("").isOneOfPrefixes({"a", "b"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, prefix_in_empty_line_multichar) {
    EXPECT_EQ(Parser("").isOneOfPrefixes({"abcdef", "xyz"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match) {
    EXPECT_EQ(Parser("a").isOneOfPrefixes({"a", "b"}), "a");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_2) {
    EXPECT_EQ(Parser("b").isOneOfPrefixes({"a", "b", "c"}), "b");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_3) {
    EXPECT_EQ(Parser("b").isOneOfPrefixes({"a", "b"}), "b");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_multichar) {
    EXPECT_EQ(Parser("abcdef").isOneOfPrefixes({"abcdef", "xyzxyz"}), "abcdef");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_multichar_2) {
    EXPECT_EQ(Parser("abcdef").isOneOfPrefixes({"xyzxyz", "abcdef"}), "abcdef");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_multichar_3) {
    EXPECT_EQ(Parser("abcdef").isOneOfPrefixes({"xyz", "abcdef"}), "abcdef");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_multichar_4) {
    EXPECT_EQ(Parser("abcdef").isOneOfPrefixes({"abcdef", "xyzxyzghj"}), "abcdef");
}

TEST_F(ParserIsOneOfPrefixesTest, wrong_short_prefixes) {
    EXPECT_EQ(Parser("abcdef").isOneOfPrefixes({"xyz", "ghj"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, wrong_long_prefixes) {
    EXPECT_EQ(Parser("xyz").isOneOfPrefixes({"abcdef", "hjkloi"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, wrong_short_and_long_prefixes) {
    EXPECT_EQ(Parser("abcdef").isOneOfPrefixes({"xyz", "hjkloirty"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, substr_present_but_not_prefix) {
    EXPECT_EQ(Parser("abcdef").isOneOfPrefixes({"def", "bcd"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, prefix_in_empty_line_not_from_beginning) {
    Parser parser("zzz");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"a", "b"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, prefix_in_empty_line_multichar_not_from_beginning) {
    Parser parser("zzz");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"abcdef", "xyz", "a"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_not_from_beginning) {
    Parser parser("zzza");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"a", "b"}), "a");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_not_from_beginning_2) {
    Parser parser("zzzb");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"a", "b"}), "b");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_multichar_not_from_beginning) {
    Parser parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"abcdef", "qwerty"}), "abcdef");
}

TEST_F(ParserIsOneOfPrefixesTest, exact_match_multichar_not_from_beginning_2) {
    Parser parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"qwerty", "abcdef"}), "abcdef");
}

TEST_F(ParserIsOneOfPrefixesTest, wrong_short_prefix_not_from_beginning) {
    Parser parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"xyz", "qwe"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, wrong_long_prefix_not_from_beginning) {
    Parser parser("zzzxyz");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"abcdef", "qwerty"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, substr_present_but_not_prefix_not_from_beginning) {
    Parser parser("zzzabcdefqwe");
    parser.pos = 3;
    EXPECT_EQ(parser.isOneOfPrefixes({"def", "qwe"}), "");
}

TEST_F(ParserIsOneOfPrefixesTest, substr_present_but_already_read) {
    Parser parser("zzzabcdefqwe");
    parser.pos = 6;
    EXPECT_EQ(parser.isOneOfPrefixes({"zzz", "abc"}), "");
}

class ParserReadPrefixTest: public testing::Test {
};

TEST_F(ParserReadPrefixTest, beginning_read_prefix) {
    auto parser = Parser("abcdef");
    EXPECT_EQ(parser.readPrefix("abc"), "abc");
    EXPECT_EQ(parser.pos, 3);
}

TEST_F(ParserReadPrefixTest, beginning_exact_match) {
    auto parser = Parser("abcdef");
    EXPECT_EQ(parser.readPrefix("abcdef"), "abcdef");
    EXPECT_EQ(parser.pos, 6);
}

TEST_F(ParserReadPrefixTest, mid_string_read_prefix) {
    auto parser = Parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_EQ(parser.readPrefix("abc"), "abc");
    EXPECT_EQ(parser.pos, 6);
}

TEST_F(ParserReadPrefixTest, mid_string_exact_match) {
    auto parser = Parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_EQ(parser.readPrefix("abcdef"), "abcdef");
    EXPECT_EQ(parser.pos, 9);
}

TEST_F(ParserReadPrefixTest, read_non_existent_prefix_beginning) {
    auto parser = Parser("zzzabcdef");
    EXPECT_EQ(parser.readPrefix("qwerty"), "");
    EXPECT_EQ(parser.pos, 0);
}

TEST_F(ParserReadPrefixTest, read_non_existent_prefix_mid_string) {
    auto parser = Parser("zzzabcdef");
    parser.pos = 3;
    EXPECT_EQ(parser.readPrefix("qwerty"), "");
    EXPECT_EQ(parser.pos, 3);
}

TEST_F(ParserReadPrefixTest, read_non_existent_prefix_end_of_string) {
    auto parser = Parser("zzzabcdef");
    parser.pos = 9;
    EXPECT_EQ(parser.readPrefix("qwerty"), "");
    EXPECT_EQ(parser.pos, 9);
}

class ParserReadTillOneOfThePrefixesTest: public testing::Test {
};

TEST_F(ParserReadTillOneOfThePrefixesTest, read_not_existent) {
    EXPECT_ANY_THROW(Parser("abc").readTillOneOfThePrefixes({"qwe", "rty"}, false));
}

TEST_F(ParserReadTillOneOfThePrefixesTest, read_not_existent_2) {
    auto parser = Parser("abc");
    parser.pos = 3;
    EXPECT_ANY_THROW(parser.readTillOneOfThePrefixes({"qwe", "rty"}, false));
}

TEST_F(ParserReadTillOneOfThePrefixesTest, read_not_existent_3) {
    EXPECT_EQ(Parser("abc").readTillOneOfThePrefixes({"qwe", "rty"}, true), "abc");
}

TEST_F(ParserReadTillOneOfThePrefixesTest, read_not_existent_4) {
    auto parser = Parser("abc");
    parser.pos = 3;
    EXPECT_EQ(parser.readTillOneOfThePrefixes({"qwe", "rty"}, true), "");
}

TEST_F(ParserReadTillOneOfThePrefixesTest, read_till_prefix) {
    EXPECT_EQ(Parser("'''abc''' '''").readTillOneOfThePrefixes({"'''", "zzzzz"}, true), "");
}

TEST_F(ParserReadTillOneOfThePrefixesTest, read_till_prefix_2) {
    EXPECT_EQ(Parser("'''abc''' '''").readTillOneOfThePrefixes({"abc", "zzz"}, true), "'''");
}

class ParserReadTillPrefixTest: public testing::Test {
};

TEST_F(ParserReadTillPrefixTest, read_not_existent) {
    EXPECT_ANY_THROW(Parser("abc").readTillPrefix("qwe", false));
}

TEST_F(ParserReadTillPrefixTest, read_not_existent_2) {
    auto parser = Parser("abc");
    parser.pos = 3;
    EXPECT_ANY_THROW(parser.readTillPrefix("qwe", false));
}

TEST_F(ParserReadTillPrefixTest, read_not_existent_3) {
    EXPECT_EQ(Parser("abc").readTillPrefix("qwe", true), "abc");
}

TEST_F(ParserReadTillPrefixTest, read_not_existent_4) {
    auto parser = Parser("abc");
    parser.pos = 3;
    EXPECT_EQ(parser.readTillPrefix("qwe", true), "");
}

class ParserEofTest: public testing::Test {
};

TEST_F(ParserReadTillPrefixTest, eof_1) {
    auto parser = Parser("");
    EXPECT_TRUE(parser.eof());
}

TEST_F(ParserReadTillPrefixTest, eof_2) {
    auto parser = Parser("abc");
    parser.pos = 2;
    EXPECT_FALSE(parser.eof());
}

TEST_F(ParserReadTillPrefixTest, eof_3) {
    auto parser = Parser("abc");
    parser.pos = 3;
    EXPECT_TRUE(parser.eof());
}

TEST_F(ParserReadTillPrefixTest, eof_4) {
    auto parser = Parser("abc");
    parser.pos = std::string::npos;
    EXPECT_TRUE(parser.eof());
}
