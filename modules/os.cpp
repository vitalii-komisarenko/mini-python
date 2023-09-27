#include "os.h"

#include <cstdlib>
#include <filesystem>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

namespace MiniPython {

void os::make_environ() {
    env = std::make_shared<DictVariable>();

    char **c_str = environ;
    for (; *c_str; c_str++) {
        std::string str(*c_str);
        size_t pos = str.find('=');
        std::string key = str.substr(0, pos);
        std::string value = str.substr(pos + 1);
        env->set_item(NEW_STRING(key), NEW_STRING(value));
    }
}

Variable os::getenv(Variable key, Variable default_value) {
    char *value = std::getenv(std::dynamic_pointer_cast<StringVariable>(key)->value.c_str());
    if (!value) {
        return default_value;
    }
    return std::make_shared<StringVariable>(value);
}

Variable os::putenv(Variable key, Variable value) {
    setenv(std::dynamic_pointer_cast<StringVariable>(key)->value.c_str(),
           std::dynamic_pointer_cast<StringVariable>(value)->value.c_str(),
           true);
    return NONE;
}

Variable os::unsetenv(Variable key) {
    ::unsetenv(std::dynamic_pointer_cast<StringVariable>(key)->value.c_str());
    return NONE;
}

Variable os::getcwd() {
    return std::make_shared<StringVariable>(std::filesystem::current_path());
}

Variable os::chdir(Variable path) {
    std::filesystem::current_path(std::dynamic_pointer_cast<StringVariable>(path)->value);
    return NONE;
}

Variable os::listdir(Variable path) {
    std::vector<Variable> list;
    for (const auto & entry : std::filesystem::directory_iterator(std::dynamic_pointer_cast<StringVariable>(path)->value)) {
        if ((entry.path() != ".") && (entry.path() != "..")) {
            list.push_back(NEW_STRING(entry.path()));
        }
    }
    return NEW_LIST(list);
}

} // namespace MiniPython
