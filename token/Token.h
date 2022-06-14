#pragma once

#include <string>
#include <vector>

namespace MiniPython {

enum class TokenType {
    OPERATOR,
    IDENTIFIER,
    STRING,
    NUMBER,
    COMMA,
    OPENING_ROUND_BRACKET,
    CLOSING_ROUND_BRACKET,
    OPENING_SQUARE_BRACKET,
    CLOSING_SQUARE_BRACKET,
    OPENING_CURLY_BRACKET,
    CLOSING_CURY_BRACKET,
};

struct Token {
    Token(TokenType _type, const std::string &value = {});
    bool operator==(const Token &other);

    TokenType type;
    std::string value;
};

using TokenList = std::vector<Token>;

TokenList tokenizeLine(const std::string &line);

} // namespace MiniPython
