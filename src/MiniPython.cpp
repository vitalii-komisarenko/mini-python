#include "mini-python.h"
#include "LineLevelParser.h"
#include "Scope.h"

#include <fstream>

namespace MiniPython {

void runFromString(const std::string &fileContent) {
    Lines lines = stringToLines(fileContent);
    lines = processLineContinuation(lines);
    lines = removeLinesWithoutCode(lines);

    LineTree lineTree(lines);

    Scope scope(lineTree);

    scope.execute();
}

void runFromFile(const std::string &filename) {
    std::ifstream f(filename);
    std::string str((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());

    runFromString(str);
}

} // namespace MiniPython