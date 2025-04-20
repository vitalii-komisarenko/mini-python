#include "Utils.h"

namespace MiniPython {

char nibble_to_hex(int ch) {
    return (ch <= 9) ? '0' + ch : 'a' + ch - 10;
}

std::string byte_to_hex(unsigned char ch) {
    return std::string() + nibble_to_hex(ch >> 4) + nibble_to_hex(ch % 16);
}

} // namespace MiniPython
