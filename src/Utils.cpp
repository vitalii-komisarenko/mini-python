#include "Utils.h"

namespace MiniPython {

char nibble_to_hex(int ch) {
    return (ch <= 9) ? '0' + ch : 'a' + ch - 10;
}

} // namespace MiniPython
