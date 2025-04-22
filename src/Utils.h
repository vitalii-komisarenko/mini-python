#pragma once

#include <string>

namespace MiniPython {

char nibble_to_hex(int ch);
std::string byte_to_hex(unsigned char ch);

int int_from_hex_char(char ch);
std::string str_from_hex_str(const std::string &hexstr);

std::string base64_encode(const std::string &input);
std::string base64_decode(std::string input);

} // namespace MiniPython
