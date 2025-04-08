#include "Token.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

using namespace MiniPython;
using testing::ElementsAre;

static void test_no_token() {
    std::vector<std::string> lines = {
        "",
        " ",
        "\t",
        "      ",
        "\t ",
        "#",
        "  # a = 3 + 4",
    };

    for (const auto& line: lines) {
        TokenList expected = {};
        TokenList actual   = tokenizeLine(line);

        bool equal = std::equal(expected.begin(), expected.end(), actual.begin(), actual.end());
        if (!equal) {
            std::cerr << __FUNCTION__ << " failed for line: '" << line << "'\n";
        }
    }
}

static void test_single_string_parsing() {
    // format: input, expected output
    std::vector<std::pair<std::string, std::string>> data = {
        {"''", ""},
        {"\"\"", ""},
        {"'#'", "#"},
        {" ' # some text'", " # some text"},
        {"'\\\\'", "\\"},
        {"'\\t'", "\t"},
        {"'\\n'", "\n"},
        {"'\\r'", "\r"},
    };

    for (const auto& pair: data) {
        std::string line = pair.first;

        TokenList expected = { Token(TokenType::STRING, pair.second) };
        TokenList actual   = tokenizeLine(line);
        bool equal = std::equal(expected.begin(), expected.end(), actual.begin(), actual.end());
        if (!equal) {
            std::cerr << __FUNCTION__ << " failed for line: '" << line << "'\n";
        }
    }
}

static void test_illegal_code() {
    std::vector<std::string> lines = {
        "'",
        "\"",
        "'''",
        "'\\'",
        "'abc'\\",
        R"('\)",
        "\\if a == b:",
    };

    for (const auto& line: lines) {
        bool exception_thrown = false;
        try {
            tokenizeLine(line);
        }
        catch (std::runtime_error) {
            exception_thrown = true;
        }

        if (!exception_thrown) {
            std::cerr << __FUNCTION__ << " failed for line: '" << line << "'\n";
        }
    }
}

static void test_misc_code() {
    std::vector<std::pair<std::string, TokenList>> input = {
        {"a = b + c", {
            {TokenType::IDENTIFIER, "a"},
            {TokenType::OPERATOR, "="},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::OPERATOR, "+"},
            {TokenType::IDENTIFIER, "c"},
        }},
        {"if 2 == 1 + 1:", {
            {TokenType::IDENTIFIER, "if"},
            {TokenType::NUMBER,     "2"},
            {TokenType::OPERATOR,   "=="},
            {TokenType::NUMBER,     "1"},
            {TokenType::OPERATOR,   "+"},
            {TokenType::NUMBER,     "1"},
            {TokenType::COLON,      ""},
        }},
        {"myVarialbe123+=\"some string\"", {
            {TokenType::IDENTIFIER, "myVarialbe123"},
            {TokenType::OPERATOR,   "+="},
            {TokenType::STRING,     "some string"},
        }},
        {R"(myString = '\n\t\\\r\'\"')", {
            {TokenType::IDENTIFIER, "myString"},
            {TokenType::OPERATOR, "="},
            {TokenType::STRING, "\n\t\\\r'\""},
        }},
        {"print('Hello, World!')", {
            {TokenType::IDENTIFIER, "print"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::STRING, "Hello, World!"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
        }},
        {"print(1 + 2)", {
            {TokenType::IDENTIFIER, "print"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::NUMBER,     "1"},
            {TokenType::OPERATOR,   "+"},
            {TokenType::NUMBER,     "2"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
        }},
        {"var = ((2+4)-(35-func(5)))//8", {
            {TokenType::IDENTIFIER, "var"},
            {TokenType::OPERATOR,   "="},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::NUMBER,     "2"},
            {TokenType::OPERATOR,   "+"},
            {TokenType::NUMBER,     "4"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
            {TokenType::OPERATOR,   "-"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::NUMBER,     "35"},
            {TokenType::OPERATOR,   "-"},
            {TokenType::IDENTIFIER, "func"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::NUMBER,     "5"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
            {TokenType::OPERATOR,   "//"},
            {TokenType::NUMBER,     "8"},
        }},
        {"2e1", {{TokenType::NUMBER, "2e1"}}},
        {"2e-1", {{TokenType::NUMBER, "2e-1"}}},
        {"2e+1", {{TokenType::NUMBER, "2e+1"}}},
        {"3.1", {{TokenType::NUMBER, "3.1"}}},
        {"56.342e-4", {{TokenType::NUMBER, "56.342e-4"}}},
        {"2.", {{TokenType::NUMBER, "2."}}},
        {".33", {{TokenType::NUMBER, ".33"}}},
        {".33E-45", {{TokenType::NUMBER, ".33E-45"}}},
        {"-1", {
            {TokenType::OPERATOR, "-"},
            {TokenType::NUMBER, "1"}
        }},
    };

    for (const auto& pair: input) {
        std::string line = pair.first;

        try {
            TokenList actual   = tokenizeLine(line);
            TokenList expected = pair.second;

            bool equal = std::equal(expected.begin(), expected.end(), actual.begin(), actual.end());
            if (!equal) {
                std::cerr << __FUNCTION__ << " failed for line: '" << line << "'\n";
            }
        }
        catch(std::runtime_error &ex) {
            std::cerr << __FUNCTION__ << " failed for line: '" << line << "'. Exception: " << ex.what() << "\n";
        }
    }
}

void test_token() {
    test_no_token();
    test_single_string_parsing();
    test_illegal_code();
    test_misc_code();
}

class TokentTest: public testing::Test {
};

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
                                                   Token(TokenType::OPENING_ROUND_BRACKET),
                                                   Token(TokenType::CLOSING_ROUND_BRACKET)));
}
