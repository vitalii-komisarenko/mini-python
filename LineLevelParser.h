#pragma once

#include <string>
#include <vector>

namespace MiniPython {

using Lines = std::vector<std::string>;

// Internal functions - exported only to be covered by tests
Lines stringToLines(const std::string &data);
Lines processLineContinuation(const Lines &lines);
bool lineHasCode(const std::string &line);
Lines removeLinesWithoutCode(const Lines &lines);

} // namespace MiniPython
