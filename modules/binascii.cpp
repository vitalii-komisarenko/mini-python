#include "Module.h"
#include "Instruction.h"
#include "Utils.h"

namespace MiniPython {

std::string binascii::helper_hexlify(const std::string &data, const std::string &sep, int bytes_per_sep) {
    std::string result;

    int bytes_till_sep = (bytes_per_sep > 0) ? (data.length() % bytes_per_sep) : -bytes_per_sep;
    if (bytes_till_sep == 0) {
        bytes_till_sep = bytes_per_sep;
    }

    bytes_per_sep = (bytes_per_sep > 0) ? bytes_per_sep : -bytes_per_sep;

    for (const char ch: data) {
        if (bytes_till_sep == 0) {
            result += sep;
            bytes_till_sep = bytes_per_sep;
        }
        bytes_till_sep--;

        result += byte_to_hex(ch);
    }

    return result;
}

static Variable hexlify(const InstructionParams &params, Scope *scope) {
    auto data = PARAM(0);
    auto sep = PARAM_DEFAULT(1, NEW_BYTES(""));
    auto bytes_per_sep = PARAM_DEFAULT(2, NEW_INT(1));

    return NEW_BYTES(binascii::helper_hexlify(VAR_TO_BYTES(data), VAR_TO_STR(sep), VAR_TO_INT(bytes_per_sep)));
}

binascii::binascii() {
    SET_FUNCTION("hexlify", hexlify);
    SET_FUNCTION("b2a_hex", hexlify);
}

} // namespace MiniPython
