#include "Token.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

using namespace MiniPython;
using testing::ElementsAre;

class TokentTest: public testing::Test {
};

TEST_F(TokentTest, no_token) {
    ASSERT_THAT(tokenizeLine(""), ElementsAre());
    ASSERT_THAT(tokenizeLine(" "), ElementsAre());
    ASSERT_THAT(tokenizeLine("\t"), ElementsAre());
    ASSERT_THAT(tokenizeLine("      "), ElementsAre());
    ASSERT_THAT(tokenizeLine("\t "), ElementsAre());
    ASSERT_THAT(tokenizeLine("#"), ElementsAre());
    ASSERT_THAT(tokenizeLine("  # a = 3 + 4"), ElementsAre());
}

TEST_F(TokentTest, single_string) {
    ASSERT_THAT(tokenizeLine("''"), ElementsAre(Token(TokenType::STRING, "")));
    ASSERT_THAT(tokenizeLine("\"\""), ElementsAre(Token(TokenType::STRING, "")));
    ASSERT_THAT(tokenizeLine("'#'"), ElementsAre(Token(TokenType::STRING, "#")));
    ASSERT_THAT(tokenizeLine(" ' # some text'"), ElementsAre(Token(TokenType::STRING, " # some text")));
    ASSERT_THAT(tokenizeLine("'\\\\'"), ElementsAre(Token(TokenType::STRING, "\\")));
    ASSERT_THAT(tokenizeLine("'\\t'"), ElementsAre(Token(TokenType::STRING, "\t")));
    ASSERT_THAT(tokenizeLine("'\\n'"), ElementsAre(Token(TokenType::STRING, "\n")));
    ASSERT_THAT(tokenizeLine("'\\r'"), ElementsAre(Token(TokenType::STRING, "\r")));
}

TEST_F(TokentTest, illegal_code) {
    EXPECT_ANY_THROW(tokenizeLine("'"));
    EXPECT_ANY_THROW(tokenizeLine("\""));
    EXPECT_ANY_THROW(tokenizeLine("'''"));
    EXPECT_ANY_THROW(tokenizeLine("'\\'"));
    EXPECT_ANY_THROW(tokenizeLine("'abc'\\"));
    EXPECT_ANY_THROW(tokenizeLine(R"('\)"));
    EXPECT_ANY_THROW(tokenizeLine("\\if a == b:"));
}

TEST_F(TokentTest, misc_code) {
    ASSERT_THAT(tokenizeLine("a = b + c"), ElementsAre(
        Token(TokenType::IDENTIFIER, "a"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::IDENTIFIER, "b"),
        Token(TokenType::OPERATOR, "+"),
        Token(TokenType::IDENTIFIER, "c")
    ));

    ASSERT_THAT(tokenizeLine("if 2 == 1 + 1:"), ElementsAre(
        Token(TokenType::IDENTIFIER, "if"),
        Token(TokenType::NUMBER,     "2"),
        Token(TokenType::OPERATOR,   "=="),
        Token(TokenType::NUMBER,     "1"),
        Token(TokenType::OPERATOR,   "+"),
        Token(TokenType::NUMBER,     "1"),
        Token(TokenType::COLON,      ":")
    ));

    ASSERT_THAT(tokenizeLine("myVarialbe123+=\"some string\""), ElementsAre(
        Token(TokenType::IDENTIFIER, "myVarialbe123"),
        Token(TokenType::OPERATOR,   "+="),
        Token(TokenType::STRING,     "some string")
    ));

    ASSERT_THAT(tokenizeLine(R"(myString = '\n\t\\\r\'\"')"), ElementsAre(
        Token(TokenType::IDENTIFIER, "myString"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::STRING, "\n\t\\\r'\"")
    ));

    ASSERT_THAT(tokenizeLine("print('Hello, World!')"), ElementsAre(
        Token(TokenType::IDENTIFIER, "print"),
        Token(TokenType::OPENING_ROUND_BRACKET, "("),
        Token(TokenType::STRING, "Hello, World!"),
        Token(TokenType::CLOSING_ROUND_BRACKET, ")")
    ));

    ASSERT_THAT(tokenizeLine("print(1 + 2)"), ElementsAre(
        Token(TokenType::IDENTIFIER, "print"),
        Token(TokenType::OPENING_ROUND_BRACKET, "("),
        Token(TokenType::NUMBER,     "1"),
        Token(TokenType::OPERATOR,   "+"),
        Token(TokenType::NUMBER,     "2"),
        Token(TokenType::CLOSING_ROUND_BRACKET, ")")
    ));

    ASSERT_THAT(tokenizeLine("var = ((2+4)-(35-func(5)))//8"), ElementsAre(
        Token(TokenType::IDENTIFIER, "var"),
        Token(TokenType::OPERATOR,   "="),
        Token(TokenType::OPENING_ROUND_BRACKET, "("),
        Token(TokenType::OPENING_ROUND_BRACKET, "("),
        Token(TokenType::NUMBER,     "2"),
        Token(TokenType::OPERATOR,   "+"),
        Token(TokenType::NUMBER,     "4"),
        Token(TokenType::CLOSING_ROUND_BRACKET, ")"),
        Token(TokenType::OPERATOR,   "-"),
        Token(TokenType::OPENING_ROUND_BRACKET, "("),
        Token(TokenType::NUMBER,     "35"),
        Token(TokenType::OPERATOR,   "-"),
        Token(TokenType::IDENTIFIER, "func"),
        Token(TokenType::OPENING_ROUND_BRACKET, "("),
        Token(TokenType::NUMBER,     "5"),
        Token(TokenType::CLOSING_ROUND_BRACKET, ")"),
        Token(TokenType::CLOSING_ROUND_BRACKET, ")"),
        Token(TokenType::CLOSING_ROUND_BRACKET, ")"),
        Token(TokenType::OPERATOR,   "//"),
        Token(TokenType::NUMBER,     "8")
    ));

    ASSERT_THAT(tokenizeLine("2e1"), ElementsAre(Token(TokenType::NUMBER, "2e1")));
    ASSERT_THAT(tokenizeLine("2e-1"), ElementsAre(Token(TokenType::NUMBER, "2e-1")));
    ASSERT_THAT(tokenizeLine("2e+1"), ElementsAre(Token(TokenType::NUMBER, "2e+1")));
    ASSERT_THAT(tokenizeLine("3.1"), ElementsAre(Token(TokenType::NUMBER, "3.1")));
    ASSERT_THAT(tokenizeLine("56.342e-4"), ElementsAre(Token(TokenType::NUMBER, "56.342e-4")));
    ASSERT_THAT(tokenizeLine("2."), ElementsAre(Token(TokenType::NUMBER, "2.")));
    ASSERT_THAT(tokenizeLine(".33"), ElementsAre(Token(TokenType::NUMBER, ".33")));
    ASSERT_THAT(tokenizeLine(".33E-45"), ElementsAre(Token(TokenType::NUMBER, ".33E-45")));

    ASSERT_THAT(tokenizeLine("-1"), ElementsAre(
        Token(TokenType::OPERATOR, "-"),
        Token(TokenType::NUMBER, "1")
    ));
}

TEST_F(TokentTest, f_string) {
    ASSERT_THAT(tokenizeLine("f'{my_var}'"), ElementsAre(Token(TokenType::FSTRING, "{my_var}")));
    ASSERT_THAT(tokenizeLine("F\"{x}\""), ElementsAre(Token(TokenType::FSTRING, "{x}")));
    ASSERT_THAT(tokenizeLine("f'{a}' + '123'"),
                ElementsAre(Token(TokenType::FSTRING, "{a}"),
                            Token(TokenType::OPERATOR, "+"),
                            Token(TokenType::STRING, "123")));
}

TEST_F(TokentTest, final_number_0) {
    ASSERT_THAT(tokenizeLine("x = 0"), ElementsAre(
        Token(TokenType::IDENTIFIER, "x"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::NUMBER, "0")
    ));
}

TEST_F(TokentTest, octal_string) {
    ASSERT_THAT(tokenizeLine("x = 0o5134"), ElementsAre(
        Token(TokenType::IDENTIFIER, "x"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::NUMBER, "0o5134")
    ));
}

TEST_F(TokentTest, hex_string) {
    ASSERT_THAT(tokenizeLine("x = 0x5A3F"), ElementsAre(
        Token(TokenType::IDENTIFIER, "x"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::NUMBER, "0x5A3F")
    ));
}

TEST_F(TokentTest, underscores_in_numbers) {
    ASSERT_THAT(tokenizeLine("1_000_000_000_000_000"),
                ElementsAre(Token(TokenType::NUMBER, "1000000000000000")));

    ASSERT_THAT(tokenizeLine("123_4_56e-0_1"),
                ElementsAre(Token(TokenType::NUMBER, "123456e-01")));

    ASSERT_THAT(tokenizeLine("0o123_456"),
                ElementsAre(Token(TokenType::NUMBER, "0o123456")));

    ASSERT_THAT(tokenizeLine("0x_FF_FF_FF_FF"),
                ElementsAre(Token(TokenType::NUMBER, "0xFFFFFFFF")));
}

TEST_F(TokentTest, attribute_access) {
    ASSERT_THAT(tokenizeLine("a.b()"), ElementsAre(Token(TokenType::IDENTIFIER, "a"),
                                                   Token(TokenType::OPERATOR, "."),
                                                   Token(TokenType::IDENTIFIER, "b"),
                                                   Token(TokenType::OPENING_ROUND_BRACKET, "("),
                                                   Token(TokenType::CLOSING_ROUND_BRACKET, ")")));
}
