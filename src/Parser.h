#pragma once

#include <string>
#include <vector>

namespace MiniPython {

class Parser {
public:
    Parser(const std::string &_buffer);

    bool isPrefix(const std::string &expected_prefix);
    std::string isOneOfPrefixes(const std::vector<std::string> &prefixes);
    std::string readPrefix(const std::string &prefix);

    std::string readTillOneOfThePrefixes(const std::vector<std::string> &prefixes,
                                         bool not_found_is_ok);
    std::string readTillPrefix(const std::string &prefix,
                               bool not_found_is_ok);
    bool eof();

    std::string buffer;
    std::string::size_type pos;
};

} // namespace MiniPython
