#include "TokenToVariable.h"

#include <stdexcept>

#define VAR(TYPE, VALUE) \
    std::static_pointer_cast<GenericVariable>(std::make_shared<TYPE ## Variable>(VALUE))

namespace MiniPython {

Variable parseTokenToVariable(const Token &token) {
    switch (token.type) {
    case TokenType::STRING:
        return VAR(String, token.value);
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
            return VAR(Float, std::stod(token.value));
        }
        else {
            int base = isOct ? 8 : isHex ? 16 : 10;

            // remove 0x / 0o prefix if needed
            auto str_value = base == 10 ? token.value : token.value.substr(2);

            return VAR(Int, std::stoull(str_value, nullptr, base));
        }
    }
    default:
        throw std::runtime_error("Can't convert token to variable");
    }
}

} // namespace MiniPython
