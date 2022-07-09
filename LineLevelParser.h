#pragma once

#include <string>
#include <memory>
#include <vector>

namespace MiniPython {

using Lines = std::vector<std::string>;

// Internal functions - exported only to be covered by tests
Lines stringToLines(const std::string &data);
Lines processLineContinuation(const Lines &lines);
bool lineHasCode(const std::string &line);
Lines removeLinesWithoutCode(const Lines &lines);

struct LineTree {
    LineTree(const Lines &line);
    LineTree(Lines::const_iterator &curr, const Lines::const_iterator &end, const std::string &parentIndentation);
    std::string indentation;
    std::string value;
    std::vector<std::shared_ptr<LineTree>> children;
};

} // namespace MiniPython
