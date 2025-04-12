#include "Parser.h"

#include <stdexcept>

namespace MiniPython {

Parser::Parser(const std::string &_buffer): buffer(_buffer), pos(0) {}

bool Parser::isPrefix(const std::string &expected_prefix) {
    return buffer.substr(pos, expected_prefix.size()) == expected_prefix;
}

std::string Parser::isOneOfPrefixes(const std::vector<std::string> &prefixes) {
    for (const auto &prefix: prefixes) {
        if (isPrefix(prefix)) {
            return prefix;
        }
    }
    return "";
}

std::string Parser::readPrefix(const std::string &prefix) {
    if (isPrefix(prefix)) {
        pos += prefix.size();
        return prefix;
    }
    return "";
}

std::string Parser::readTillOneOfThePrefixes(const std::vector<std::string> &prefixes,
                                             bool not_found_is_ok) {
    auto found_pos = std::string::npos;

    for (const auto& prefix: prefixes) {
        auto new_pos = buffer.find(prefix, pos);
        if (new_pos < found_pos) {
            found_pos = new_pos;
        }
    }

    if ((found_pos == std::string::npos) && !not_found_is_ok) {
        throw std::runtime_error("Can't find prefix");
    }

    std::string result = buffer.substr(pos, found_pos - pos);
    pos = found_pos;
    return result;
}

std::string Parser::readTillPrefix(const std::string &prefix,
                                   bool not_found_is_ok) {
    std::vector prefixes = {prefix};
    return readTillOneOfThePrefixes(prefixes, not_found_is_ok);
}

bool Parser::eof() {
    return pos >= buffer.length();
}

} // namespace MiniPython
