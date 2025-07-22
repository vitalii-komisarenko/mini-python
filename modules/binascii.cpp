#include "Module.h"
#include "FunctionParamatersParsing.h"
#include "Instruction.h"
#include "RaiseException.h"
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

static Variable b2a_base64(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"data", "newline"},
        {
            {"newline", TRUE},
        }
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);
    auto data = VAR_TO_BYTES(parsed_params.vars["data"]);
    auto newline = parsed_params.vars["newline"]->to_bool() ? "\n" : "";

    return NEW_BYTES(base64_encode(data) + newline);
}

static Variable a2b_base64(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"string", "strict_mode"},
        {{"strict_mode", FALSE}}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);
    auto string = VAR_TO_STR(parsed_params.vars["string"]);
    auto strict_mode = parsed_params.vars["strict_mode"]->to_bool();

    if (strict_mode) {
        if (string.size() && (string[0] == '=')) {
            raise_exception("binascii.Error", "Leading padding not allowed");
        }

        if (string.size() % 4) {
            raise_exception("binascii.Error", "Incorrect padding");
        }

        size_t last_padding_pos = string.rfind('=');
        if ((last_padding_pos != std::string::npos) && (last_padding_pos != string.size() - 1)) {
            raise_exception("binascii.Error", "Discontinuous padding not allowed");
        }
    }

    return NEW_BYTES(base64_decode(string));
}

binascii::binascii() {
    SET_FUNCTION("hexlify", hexlify);
    SET_FUNCTION("b2a_hex", hexlify);
    SET_FUNCTION("unhexlify", unhexlify);
    SET_FUNCTION("a2b_hex", unhexlify);

    SET_FUNCTION("b2a_base64", b2a_base64);
    SET_FUNCTION("a2b_base64", a2b_base64);
}

} // namespace MiniPython
