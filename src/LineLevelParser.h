#pragma once

#include <string>
#include <memory>
#include <vector>

namespace MiniPython {

using Lines = std::vector<std::string>;

// Internal functions - exported only to be covered by tests
std::string replace_all(const std::string &input,
                        const std::string &pattern,
                        const std::string &repalcement);
Lines stringToLines(const std::string &data);
Lines processLineContinuation(const Lines &lines);
bool lineHasCode(const std::string &line);
Lines removeLinesWithoutCode(const Lines &lines);

struct LineTree {
    LineTree(const Lines &line);
    LineTree(Lines::const_iterator &curr, const Lines::const_iterator &end, const std::string &parentIndentation);
    LineTree(const std::string &file_content);
    std::string indentation;
    std::string value;
    std::vector<std::shared_ptr<LineTree>> children;
};

} // namespace MiniPython
