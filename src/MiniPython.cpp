#include "mini-python.h"
#include "LineLevelParser.h"
#include "Scope.h"
#include "StandardFunctions.h"

#include <fstream>

namespace MiniPython {

void runFromString(const std::string &fileContent) {
    Lines lines = stringToLines(fileContent);
    lines = processLineContinuation(lines);
    lines = removeLinesWithoutCode(lines);

    LineTree lineTree(lines);
    auto scope = makeScope(lineTree);

    scope->setVariable("print", std::make_shared<FunctionVariable>(StandardFunctions::print));
    scope->setVariable("min", std::make_shared<FunctionVariable>(StandardFunctions::min));
    scope->setVariable("max", std::make_shared<FunctionVariable>(StandardFunctions::max));
    scope->setVariable("pow", std::make_shared<FunctionVariable>(StandardFunctions::max));
    scope->setVariable("bool", std::make_shared<FunctionVariable>(StandardFunctions::bool_func));
    scope->setVariable("hex", std::make_shared<FunctionVariable>(StandardFunctions::hex));
    scope->setVariable("ord", std::make_shared<FunctionVariable>(StandardFunctions::ord));

    scope->execute();
}

void runFromFile(const std::string &filename) {
    std::ifstream f(filename);
    std::string str((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());

    runFromString(str);
}

} // namespace MiniPython
