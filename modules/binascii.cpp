#include "Module.h"
#include "FunctionParamatersParsing.h"
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
        if ((bytes_till_sep == 0) && (bytes_per_sep != 0)) {
            result += sep;
            bytes_till_sep = bytes_per_sep;
        }
        bytes_till_sep--;

        result += byte_to_hex(ch);
    }

    return result;
}

static Variable hexlify(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"data", "sep", "bytes_per_sep"},
        {
            {"sep", NEW_BYTES("")},
            {"bytes_per_sep", NEW_INT(1)},
        }
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);
    auto data = VAR_TO_BYTES(parsed_params.vars["data"]);
    auto sep = VAR_TO_STR(parsed_params.vars["sep"]);
    auto bytes_per_sep = VAR_TO_INT(parsed_params.vars["bytes_per_sep"]);

    return NEW_BYTES(binascii::helper_hexlify(data, sep, bytes_per_sep));
}

static Variable unhexlify(const InstructionParams &params, Scope *scope) {
    auto hexstr = VAR_TO_STR(PARAM(0));
    return NEW_BYTES(str_from_hex_str(hexstr));
}

binascii::binascii() {
    SET_FUNCTION("hexlify", hexlify);
    SET_FUNCTION("b2a_hex", hexlify);
    SET_FUNCTION("unhexlify", unhexlify);
    SET_FUNCTION("a2b_hex", unhexlify);
}

} // namespace MiniPython
