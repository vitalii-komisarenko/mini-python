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

        if (isFloat) {
            return VAR(Float, std::stod(token.value));
        }
        else {
            return VAR(Int, std::stod(token.value));
        }
    }
    default:
        throw std::runtime_error("Can't convert token to variable");
    }
}

} // namespace MiniPython
