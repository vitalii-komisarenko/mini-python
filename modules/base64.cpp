#include "Module.h"
#include "Instruction.h"
#include "FunctionParamatersParsing.h"
#include "Utils.h"

namespace MiniPython {

static std::string remove_non_base64_characters(const std::string &input) {
    std::string result;
    for (char ch: input) {
        bool is_base64_char = false;

        if (('A' <= ch) && (ch <= 'Z')) {
            is_base64_char = true;
        }
        
        if (('a' <= ch) && (ch <= 'z')) {
            is_base64_char = true;
        }
        
        if (('0' <= ch) && (ch <= '9')) {
            is_base64_char = true;
        }
        
        if (ch == '+') {
            is_base64_char = true;
        }
        
        if (ch == '/') {
            is_base64_char = true;
        }

        if (ch == '=') {
            is_base64_char = true;
        }

        if (is_base64_char) {
            result += ch;
        }
    }

    return result;
}

static Variable decodebytes(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"s"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    std::string input = VAR_TO_STR(parsed_params.vars["s"]->to_bytes_variable());
    input = remove_non_base64_characters(input);
    return NEW_BYTES(base64_decode(input));
}

static Variable encodebytes(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"s"},
        {}
    };
    
    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    std::string simple_encoded = base64_encode(VAR_TO_STR(parsed_params.vars["s"]));

    // Insert a newline every 76 bytes (RFC 2045)
    for (size_t next_insertion_pos = 76; next_insertion_pos < simple_encoded.size(); next_insertion_pos += 77) {
        simple_encoded.insert(next_insertion_pos, "\n");
    }

    // Ensure the input ends up with a newline (RFC 2045)
    if (simple_encoded.size() && (simple_encoded[simple_encoded.size() - 1] != '\n')) {
        simple_encoded += '\n';
    }

    return NEW_BYTES(simple_encoded);
}

base64::base64() {
    SET_FUNCTION("decodebytes", decodebytes);
    SET_FUNCTION("encodebytes", encodebytes);
}

} // namespace MiniPython
