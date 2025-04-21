#include "Utils.h"

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

} // namespace MiniPython
