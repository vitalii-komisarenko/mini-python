#include "Token.h"

#include <iostream>

using namespace MiniPython;

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
    };
    //TODO
}

int main() {
    test_no_token();
    test_single_string_parsing();
    test_illegal_code();
    test_misc_code();
}
