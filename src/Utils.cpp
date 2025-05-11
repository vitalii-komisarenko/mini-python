#include "Utils.h"

#include <algorithm>
#include <stdexcept>

namespace MiniPython {

char nibble_to_hex(int ch) {
    return (ch <= 9) ? '0' + ch : 'a' + ch - 10;
}

std::string byte_to_hex(unsigned char ch) {
    return std::string() + nibble_to_hex(ch >> 4) + nibble_to_hex(ch % 16);
}

int int_from_hex_char(char ch) {
    if (('0' <= ch) && (ch <= '9')) {
        return ch - '0';
    }
    if (('a' <= ch) && (ch <= 'f')) {
        return ch - 'a' + 10;
    }
    if (('A' <= ch) && (ch <= 'F')) {
        return ch - 'A' + 10;
    }
    throw std::runtime_error("Not a hex character");
}

std::string str_from_hex_str(const std::string &hexstr) {
    std::string str_without_spaces;
    for (size_t i = 0; i < hexstr.size(); ++i) {
        if ((hexstr[i] != ' ') && (hexstr[i] != '\t')) {
            str_without_spaces += hexstr[i];
        }
    }

    if (str_without_spaces.size() % 2) {
        throw std::runtime_error("Even number of chars expected");
    }

    std::string result;
    for (size_t i = 0 ; i < str_without_spaces.size(); i += 2) {
        char ch1 = str_without_spaces[i];
        char ch2 = str_without_spaces[i+1];
        result += (16 * int_from_hex_char(ch1) + int_from_hex_char(ch2));
    }

    return result;
}

static const char* BASE64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const std::string &input) {
    std::string result;

    unsigned char prev_value;
    for (size_t i = 0; i < input.size(); ++i) {
        if (i % 3 == 0) {
            result += BASE64_ALPHABET[input[i] >> 2];
            prev_value = input[i] & 0b00000011;
        }
        else if (i % 3 == 1) {
            result += BASE64_ALPHABET[(input[i] >> 4) | (prev_value << 4)];
            prev_value = input[i] & 0b00001111;
        }
        else {
            result += BASE64_ALPHABET[(input[i] >> 6) | (prev_value << 2)];
            result += BASE64_ALPHABET[input[i] & 0b00111111];
        }
    }

    if (input.size() % 3 == 1) {
        result += BASE64_ALPHABET[prev_value << 4];
        result += "==";
    }
    else if (input.size() % 3 == 2) {
        result += BASE64_ALPHABET[prev_value << 2];
        result += "=";
    }

    return result;
}

static unsigned char base64_decode_char(unsigned char ch) {
    if (('A' <= ch) && (ch <= 'Z')) {
        return ch - 'A';
    }
    if (('a' <= ch) && (ch <= 'z')) {
        return ch - 'a' + 26;
    }
    if (('0' <= ch) && (ch <= '9')) {
        return ch - '0' + 52;
    }
    if (ch == '+') {
        return 62;
    }
    if (ch == '/') {
        return 63;
    }
    throw std::runtime_error(std::string("Unexpected argument of base64_decode_char: '") + ((char)ch) + "'");
}

std::string base64_decode(std::string input) {
    // binascii.a2b_base64() in non-strict mode ignores leading padding
    // base64.decodebytes() does the same
    while(input.size() && (input[0] == '=')) {
        input = input.substr(1);
    }

    // binascii.a2b_base64 ignores data after padding
    // So remove everything after padding started
    size_t first_equal_sign_pos = input.find('=');
    if (first_equal_sign_pos != std::string::npos) {
        input = input.substr(0, first_equal_sign_pos);
    }

    // And since we removed the padding, add it back
    while (input.size() % 4) {
        input += '=';
    }

    std::string result;

    for (size_t i = 0; i < input.size(); i += 4) {
        result += ((base64_decode_char(input[i]) << 2) | (base64_decode_char(input[i + 1]) >> 4));

        if (input[i + 2] == '=') {
            break;
        }

        result += (base64_decode_char(input[i + 1]) << 4) | (base64_decode_char(input[i + 2]) >> 2);

        if (input[i + 3] == '=') {
            break;
        }

        result += (base64_decode_char(input[i + 2]) << 6) | (base64_decode_char(input[i + 3]));
    }

    return result;
}

} // namespace MiniPython
