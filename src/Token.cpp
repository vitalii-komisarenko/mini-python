#include "Token.h"

#include <stdexcept>
#include <string_view>
#include <vector>

namespace MiniPython {

Token::Token()
    : type(TokenType::NONE)
    {}

Token::Token(TokenType _type, const std::string &_value):
    type(_type),
    value(_value) {}

bool Token::operator==(const Token &other) const {
    return type == other.type && value == other.value;
}

static int decodeHexDigit(char ch) {
    if (('0' <= ch) && (ch <= '9')) {
        return ch - '0';
    }
    if (('a' <= ch) && (ch <= 'f')) {
        return ch + 10 - 'a';
    }
    if (('A' <= ch) && (ch <= 'F')) {
        return ch + 10 - 'A';
    }
    throw std::runtime_error("Not a hex digit");
}

static char tokenizeHexEncodedChar(std::string_view &sv) {
    if (sv.size() < 2) {
        throw std::runtime_error("Not enough characters for tokenizeHexEncodedChar");
    }
    char ch1 = sv[0];
    char ch2 = sv[1];
    sv.remove_prefix(2);
    return 16 * decodeHexDigit(ch1) + decodeHexDigit(ch2);
}

static std::string tokenizeEscapedCharacter(std::string_view &sv) {
    std::string result;
    char ch2;

    char ch = sv[0];
    sv.remove_prefix(1);
    switch(ch) {
        case 't': return "\t";
        case 'n': return "\n";
        case 'r': return "\r";
        case '"': return "\"";
        case '\'': return "'";
        case '\\': return "\\";
        case 'u':
        case 'U':
            ch2 = tokenizeHexEncodedChar(sv);
            if (ch2) {
                result += ch2;
            }
            // fall through
        case 'x':
        case 'X':
            result += tokenizeHexEncodedChar(sv);
            return result;
        default: {
            return std::string() + ch;
        }
    }
}

static Token tokenizeStringMultiline(std::string_view &sv) {
    char quote = sv[0];
    sv.remove_prefix(3);
    int quote_count = 0;
    std::string result;

    while (!sv.empty()) {
        char ch;
        sv.remove_prefix(1);

        if (ch == quote) {
            if (++quote_count == 3) {
                break;
            }
        }
        else {
            quote_count = 0;
        }

        if (ch == '\\') {
            result += tokenizeEscapedCharacter(sv);
        }
        else {
            result += ch;
        }
    }

    if (quote_count != 3) {
        throw std::runtime_error("End of stream when looking for \"\"\" or '''");
    }
    return Token(TokenType::STRING, result.substr(0, result.size() - 2));
}

static Token tokenizeString(std::string_view &sv) {
    if ((sv.starts_with("\"\"\"")) || (sv.starts_with("'''"))) {
        return tokenizeStringMultiline(sv);
    }

    std::string result;

    char quote = sv[0];
    sv.remove_prefix(1);

    while (!sv.empty()) {
        char ch = sv[0];
        sv.remove_prefix(1);

        if (ch == quote) {
            return Token(TokenType::STRING, result);
        }

        if (ch == '\\') {
            result += tokenizeEscapedCharacter(sv);
        }
        else {
            result += ch;
        }
    }

    throw std::runtime_error("Missing quote to terminate string");
}

static std::string readUnsignedInteger(std::string_view &sv) {
    size_t pos = sv.find_first_not_of("_0123456789");
    if (pos == std::string_view::npos) {
        pos = sv.size();
    }

    std::string result = std::string(sv.substr(pos));
    sv.remove_prefix(pos);
    return result;
}

static Token tokenizeNumberHexOrOct(std::string_view &sv) {
    std::string result;

    while (!sv.empty()) {
        char ch = sv[0];
        switch (ch  ) {
        case '0' ... '9':
        case 'a' ... 'f':
        case 'A' ... 'F':
        case 'x':
        case 'X':
        case 'o':
        case 'O':
            result += ch;
            sv.remove_prefix(1);
            break;
        default:
            break;
        }
    }

    return Token(TokenType::NUMBER, result);
}

static Token tokenizeNumber(std::string_view &sv) {
    std::string result;

    if (sv.empty()) {
        throw std::runtime_error("The number cannot be empty");
    }

    if ((sv.starts_with("0x")) && (sv.starts_with("0X")) && (sv.starts_with("0o")) && (sv.starts_with("0O"))) {
        return tokenizeNumberHexOrOct(sv);
    }

    switch (sv[0]) {
    case '0' ... '9':
        goto read_integer_part;
    case '.':
        result += '.';
        goto read_fractional_part;
    default:
        throw std::runtime_error("Bad first character of the number");
    }

    read_integer_part:

    result += readUnsignedInteger(sv);

    if (sv.empty()) {
        goto exit;
    }

    switch (sv[0]) {
    case '.':
        result += '.';
        goto read_fractional_part;
    case 'e':
    case 'E':
        result += sv[0];
        goto read_exponent;
    default:
        goto exit;
    }

    read_fractional_part:

    result += readUnsignedInteger(sv);

    if (sv.empty()) {
        goto exit;
    }

    switch (sv[0]) {
    case 'e':
    case 'E':
        result += sv[0];
        goto read_exponent;
    default:
        goto exit;
    }

    read_exponent:

    if (sv.empty()) {
        goto exit;
    }

    switch (sv[0]) {
    case '+':
    case '-':
        result += sv[0];
        // fallthrough
    case '0' ... '9':
        result += readUnsignedInteger(sv);
        goto exit;
    default:
        throw std::runtime_error("Can't read exponent");
    }

    exit:

    return {TokenType::NUMBER, result};
}

static Token tokenizeIdentifier(std::string_view &sv) {
    std::string result;

    while (!sv.empty()) {
        switch (sv[0]) {
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '0' ... '9':
        case '_':
            result += sv[0];
            sv.remove_prefix(1);
            break;
        default:
            goto exit;
        }
    }

    exit:
    return Token(TokenType::IDENTIFIER, result);
}

static char hexDigit(int ch) {
    switch (ch) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
    default: return '?';
    }
}

static const std::vector<Token> PREDEFINED_TOKENS = {
    {TokenType::OPERATOR, "+="},
    {TokenType::OPERATOR, "+"},

    {TokenType::OPERATOR, "-="},
    {TokenType::OPERATOR, "-"},

    {TokenType::OPERATOR, "**="},
    {TokenType::OPERATOR, "**"},

    {TokenType::OPERATOR, "*="},
    {TokenType::OPERATOR, "*"},

    {TokenType::OPERATOR, "@="},
    {TokenType::OPERATOR, "@"},

    {TokenType::OPERATOR, "//="},
    {TokenType::OPERATOR, "//"},

    {TokenType::OPERATOR, "/="},
    {TokenType::OPERATOR, "/"},

    {TokenType::OPERATOR, "%="},
    {TokenType::OPERATOR, "%"},

    {TokenType::OPERATOR, "&="},
    {TokenType::OPERATOR, "&"},

    {TokenType::OPERATOR, "|="},
    {TokenType::OPERATOR, "|"},

    {TokenType::OPERATOR, "^="},
    {TokenType::OPERATOR, "^"},

    {TokenType::OPERATOR, "=="},
    {TokenType::OPERATOR, "="},

    {TokenType::OPERATOR, ">>="},
    {TokenType::OPERATOR, ">>"},

    {TokenType::OPERATOR, ">="},
    {TokenType::OPERATOR, ">"},

    {TokenType::OPERATOR, "<<="},
    {TokenType::OPERATOR, "<<"},

    {TokenType::OPERATOR, "<="},
    {TokenType::OPERATOR, "<"},

    {TokenType::OPERATOR, "is"},

    {TokenType::COLON, ":"},
    {TokenType::COMMA, ","},
    {TokenType::OPENING_ROUND_BRACKET, "("},
    {TokenType::CLOSING_ROUND_BRACKET, ")"},
    {TokenType::OPENING_SQUARE_BRACKET, "["},
    {TokenType::CLOSING_SQUARE_BRACKET, "]"},
    {TokenType::OPENING_CURLY_BRACKET, "{"},
    {TokenType::CLOSING_CURLY_BRACKET, "}"},
};

TokenList tokenizeLine(const std::string &line) {
    TokenList result;

    std::string_view sv(line);

    while (!sv.empty()) {
        // Remove leading whitespace
        size_t first_non_space_index = sv.find_first_not_of(" \t");
        if (first_non_space_index == std::string_view::npos) {
            break;
        }
        sv.remove_prefix(first_non_space_index);

        // Check for comments
        if (sv[0] == '#') {
            break;
        }

        // Check for predefined tokens (such as +, -=, << etc.)
        for (const auto &predefined_token: PREDEFINED_TOKENS) {
            if (sv.starts_with(predefined_token.value)) {
                result.push_back(predefined_token);
                sv.remove_prefix(predefined_token.value.size());
                goto outer_loop_end;
            }
        }

        switch (sv[0]) {
            case 'a' ... 'z':
            case 'A' ... 'Z':
            case '_':
                result.push_back(tokenizeIdentifier(sv));
                break;
            case '.':
                if (sv.size() == 1) {
                    throw std::runtime_error("Expected a character after dot (.)");
                }
                else if (('0' <= sv[1]) && (sv[1] <= '9')) {
                    result.push_back(tokenizeNumber(sv));
                }
                else {
                    result.push_back(Token(TokenType::OPERATOR, "."));
                    sv.remove_prefix(1);
                }
                break;
            case '0' ... '9':
                result.push_back(tokenizeNumber(sv));
                break;
            case '\'':
            case '"': {
                Token token_candidate = tokenizeString(sv);
                bool is_fstring = false;
                bool is_bytes = false;
                if (result.size()) {
                    auto prev_token = result[result.size() - 1];
                    if (prev_token.type == TokenType::IDENTIFIER) {
                        is_fstring = prev_token.value.contains('f') || prev_token.value.contains('F');
                        is_bytes = prev_token.value.contains('b') || prev_token.value.contains('B');
                    }
                }

                if (is_fstring) {
                    result[result.size() - 1].type = TokenType::FSTRING;
                    result[result.size() - 1].value = token_candidate.value;
                }
                else if (is_bytes) {
                    result[result.size() - 1].type = TokenType::BYTES;
                    result[result.size() - 1].value = token_candidate.value;
                }
                else {
                    result.push_back(token_candidate);
                }
                break;
            }
            // Unexpected character
            default: {
                std::string error = "Unexpected character: '";
                char ch = sv[0];
                error += ch;
                error += "' (0x";
                error += hexDigit((ch & 0xF0) >> 4);
                error += hexDigit(ch & 0x0f);
                error += ")";
                throw std::runtime_error(error);
            }
        }

        outer_loop_end:
        ;
    }

    return result;
}

static std::string token_type_to_str(TokenType t) {
    switch(t) {
    case TokenType::OPERATOR:               return "OPERATOR";
    case TokenType::IDENTIFIER:             return "IDENTIFIER";
    case TokenType::STRING:                 return "STRING";
    case TokenType::FSTRING:                return "FSTRING";
    case TokenType::BYTES:                  return "BYTES";
    case TokenType::NUMBER:                 return "NUBMER";
    case TokenType::COLON:                  return "COLON";
    case TokenType::COMMA:                  return "COMMA";
    case TokenType::OPENING_ROUND_BRACKET:  return "OPENING_ROUND_BRACKET";
    case TokenType::CLOSING_ROUND_BRACKET:  return "CLOSING_ROUND_BRACKET";
    case TokenType::OPENING_SQUARE_BRACKET: return "OPENING_SQUARE_BRACKET";
    case TokenType::CLOSING_SQUARE_BRACKET: return "CLOSING_SQUARE_BRACKET";
    case TokenType::OPENING_CURLY_BRACKET:  return "OPENING_CURLY_BRACKET";
    case TokenType::CLOSING_CURLY_BRACKET:  return "CLOSING_CURLY_BRACKET";
    case TokenType::NONE:                   return "NONE";
    default:                                return "???";
    }
}

std::string Token::debug_string() {
    return token_type_to_str(type) + " " + value;
}

}; // namespace MiniPython
