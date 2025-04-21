#include "TokenToVariable.h"

#include <stdexcept>

namespace MiniPython {

Variable parseTokenToVariable(const Token &token) {
    switch (token.type) {
    case TokenType::STRING:
        return NEW_STRING(token.value);
    case TokenType::BYTES:
        return NEW_BYTES(token.value);
    case TokenType::NUMBER: {
        // TODO - better type detection
        bool isFloat = (token.value.find('.') != std::string::npos)
                    || (token.value.find('e') != std::string::npos)
                    || (token.value.find('E') != std::string::npos);

        bool isOct = (token.value.find('o') != std::string::npos)
                  || (token.value.find('O') != std::string::npos);

        bool isHex = (token.value.find('x') != std::string::npos)
                  || (token.value.find('X') != std::string::npos);

        if (isFloat) {
            return NEW_FLOAT(std::stod(token.value));
        }
        else {
            int base = isOct ? 8 : isHex ? 16 : 10;

            // remove 0x / 0o prefix if needed
            auto str_value = base == 10 ? token.value : token.value.substr(2);

            return NEW_INT(std::stoull(str_value, nullptr, base));
        }
    }
    default:
        throw std::runtime_error("Can't convert token to variable");
    }
}

} // namespace MiniPython
