#include "Token.h"

#include <sstream>
#include <stdexcept>

namespace MiniPython {

Token::Token(TokenType _type, const std::string &_value):
    type(_type),
    value(_value) {}

bool Token::operator==(const Token &other) {
    return type == other.type && value == other.value;
}

static Token tokenizeString(std::stringstream &ss) {
    std::string result;

    char quote = ss.get();

    char ch;
    while (ss && (ch = ss.get())) {
        if (ch == quote) {
            return Token(TokenType::STRING, result);
        }

        if (ch == '\\') {
            if (ss.eof()) {
                throw std::runtime_error("End of stream after backslash when reading string");
            }
            switch(ss.get()) {
            case 't': { result += "\t"; break; }
            case 'n': { result += "\n"; break; }
            case 'r': { result += "\r"; break; }
            case '"': { result += "\""; break; }
            case '\'': { result += "'"; break; }
            case '\\': { result += "\\"; break; }
            }
        }
        else {
            result += ch;
        }
    }

    throw std::runtime_error("Missing quote to terminate string");
}

static std::string readUnsignedInteger(std::stringstream &ss) {
    std::string result;

    char ch;
    while (ss && (ch = ss.get())) {
        switch(ch) {
        case '0' ... '9': {
            result += ch;
            break;
        }
        default: {
            ss.putback(ch);
            goto exit;
        }
        }
    }

    exit:
    return result;
}

static Token tokenizeNumber(std::stringstream &ss) {
    // TODO: oct & hex numbers
    std::string result;

    switch (ss.peek()) {
    case '0' ... '9':
        goto read_integer_part;
    case '.':
        result += ss.get();
        goto read_fractional_part;
    default:
        throw std::runtime_error("Bad first character of the number");
    }

    read_integer_part:

    result += readUnsignedInteger(ss);

    switch (ss.peek()) {
    case '.':
        result += ss.get();
        goto read_fractional_part;
    case 'e':
    case 'E':
        result += ss.get();
        goto read_exponent;
    default:
        goto exit;
    }

    read_fractional_part:

    result += readUnsignedInteger(ss);

    switch (ss.peek()) {
    case 'e':
    case 'E':
        result += ss.get();
        goto read_exponent;
    default:
        goto exit;
    }

    read_exponent:

    switch (ss.peek()) {
    case '+':
    case '-':
        result += ss.get();
        // fallthrough
    case '0' ... '9':
        result += readUnsignedInteger(ss);
        goto exit;
    default:
        throw std::runtime_error("Can't read exponent");
    }

    exit:

    return {TokenType::NUMBER, result};
}

static Token tokenizeIdentifier(std::stringstream &ss) {
    std::string result;

    while (ss) {
        switch (ss.peek()) {
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '0' ... '9':
        case '_':
            result += ss.get();
            break;
        case std::char_traits<char>::eof():
            goto exit;
        default:
            goto exit;
        }
    }

    exit:
    return Token(TokenType::IDENTIFIER, result);
}

static Token tokenizeOperator(std::stringstream &ss) {
    std::string result;

    while (ss) {
        switch (ss.peek()) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '<':
        case '>':
            result += ss.get();
            break;
        case std::char_traits<char>::eof():
            goto exit;
        default:
            goto exit;
        }
    }

    exit:
    return Token(TokenType::OPERATOR, result);
}

static void discardCharacter(std::stringstream &ss) {
    ss.get();
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

TokenList tokenizeLine(const std::string &line) {
    TokenList result;

    std::stringstream ss(line);
    while (ss) {
        switch (ss.peek()) {
        case ' ':
        case '\t':
            discardCharacter(ss);
            break;
        case '#':
        case std::char_traits<char>::eof():
            return result;
        case '\'':
        case '"':
            result.push_back(tokenizeString(ss));
            break;
        case '0' ... '9':
        case '.':
            result.push_back(tokenizeNumber(ss));
            break;
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':
            result.push_back(tokenizeIdentifier(ss));
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '<':
        case '>':
            result.push_back(tokenizeOperator(ss));
            break;
        case ':':
            result.push_back(Token(TokenType::COLON));
            discardCharacter(ss);
            break;
        case ',':
            result.push_back(Token(TokenType::COMMA));
            discardCharacter(ss);
            break;
        case '(':
            result.push_back(Token(TokenType::OPENING_ROUND_BRACKET));
            discardCharacter(ss);
            break;
        case ')':
            result.push_back(Token(TokenType::CLOSING_ROUND_BRACKET));
            discardCharacter(ss);
            break;
        case '[':
            result.push_back(Token(TokenType::OPENING_SQUARE_BRACKET));
            discardCharacter(ss);
            break;
        case ']':
            result.push_back(Token(TokenType::CLOSING_SQUARE_BRACKET));
            discardCharacter(ss);
            break;
        case '{':
            result.push_back(Token(TokenType::OPENING_CURLY_BRACKET));
            discardCharacter(ss);
            break;
        case '}':
            result.push_back(Token(TokenType::CLOSING_CURY_BRACKET));
            discardCharacter(ss);
            break;
        default:
            std::string error = "Unexpected character: '";
            char ch = ss.peek();
            error += ch;
            error += "' (0x";
            error += hexDigit((ch & 0xF0) >> 4);
            error += hexDigit(ch & 0x0f);
            error += ")";
            throw std::runtime_error(error);
        }
    }

    return result;
}

}; // namespace MiniPython
