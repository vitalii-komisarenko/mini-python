#include "LineLevelParser.h"

#include <iostream>

namespace MiniPython {

Lines stringToLines(const std::string &data) {
    Lines result;

    std::string current;
    for (const auto& ch: data) {
        if (ch == '\n') {
            result.push_back(current);
            current = {};
        }
        else {
            current += ch;
        }
    }

    result.push_back(current);

    return result;
}

Lines processLineContinuation(const Lines &lines) {
    Lines result;

    std::string prev_line;
    for (const auto& line: lines) {
        // line expects continuation
        if (line.size() > 0 && line[line.size() - 1] == '\\') {
            prev_line += line;
            prev_line.pop_back(); // remove trailing backslash
        }
        // normal line / continuation of previous line
        else {
            result.push_back(prev_line + line);
            prev_line = {};
        }
    }

    // last line expects continuation
    if (prev_line.size() > 0) {
        std::cerr << "Last line expects continuation (ends with '\\')\n";
        result.push_back(prev_line);
    }

    return result;
}

bool lineHasCode(const std::string &line) {
    for (const auto& ch: line) {
        if (ch == ' ' || ch == '\t') {
            continue;
        }
        else if (ch == '#') {
            return false;
        }
        else {
            return true;
        }
    }
    return false;
}

Lines removeLinesWithoutCode(const Lines &lines) {
    Lines result;

    for (const auto& line: lines) {
        if (lineHasCode(line)) {
            result.push_back(line);
        }
    }

    return result;
}

} // namespace MiniPython
