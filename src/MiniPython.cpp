#include "mini-python.h"
#include "LineLevelParser.h"
#include "Scope.h"
#include "StandardFunctions.h"
#include "modules/Module.h"

#include <fstream>

namespace MiniPython {

void runFromString(const std::string &fileContent) {
    LineTree lineTree(fileContent);
    auto scope = makeScope(lineTree);

    scope->setVariable("print", std::make_shared<FunctionVariable>(StandardFunctions::print));
    scope->setVariable("min", std::make_shared<FunctionVariable>(StandardFunctions::min));
    scope->setVariable("max", std::make_shared<FunctionVariable>(StandardFunctions::max));
    scope->setVariable("pow", std::make_shared<FunctionVariable>(StandardFunctions::pow));
    scope->setVariable("bool", std::make_shared<FunctionVariable>(StandardFunctions::bool_func));
    scope->setVariable("hex", std::make_shared<FunctionVariable>(StandardFunctions::hex));
    scope->setVariable("ord", std::make_shared<FunctionVariable>(StandardFunctions::ord));
    scope->setVariable("len", std::make_shared<FunctionVariable>(StandardFunctions::len));
    scope->setVariable("list", std::make_shared<FunctionVariable>(StandardFunctions::list));
    scope->setVariable("tuple", std::make_shared<FunctionVariable>(StandardFunctions::list));
    scope->setVariable("set", std::make_shared<FunctionVariable>(StandardFunctions::set));
    scope->setVariable("frozenset", std::make_shared<FunctionVariable>(StandardFunctions::set));
    scope->setVariable("eval", std::make_shared<FunctionVariable>(StandardFunctions::eval));

    scope->setVariable("math", std::static_pointer_cast<GenericVariable>(std::make_shared<math>()));
    scope->setVariable("os", std::static_pointer_cast<GenericVariable>(std::make_shared<os>()));
    scope->setVariable("sys", std::static_pointer_cast<GenericVariable>(std::make_shared<sys>()));
    scope->setVariable("time", std::static_pointer_cast<GenericVariable>(std::make_shared<time>()));

    scope->execute();
}

void runFromFile(const std::string &filename) {
    std::ifstream f(filename);
    std::string str((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());

    runFromString(str);
}

} // namespace MiniPython
