#include "os.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>

namespace fs = std::filesystem;

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
    std::filesystem::current_path(VAR_TO_STR(path));
    return NONE;
}

Variable os::listdir(Variable path) {
    std::vector<Variable> list;
    for (const auto & entry : std::filesystem::directory_iterator(VAR_TO_STR(path))) {
        if ((entry.path() != ".") && (entry.path() != "..")) {
            list.push_back(NEW_STRING(entry.path()));
        }
    }
    return NEW_LIST(list);
}

Variable os::mkdir(Variable path, Variable mode) {
    if (fs::exists(VAR_TO_STR(path))) {
        throw std::runtime_error("FileExistsError");
    }
    fs::create_directory(VAR_TO_STR(path));
    return NONE;
}

Variable os::makedirs(Variable path, Variable mode, Variable exists_ok) {
    if (fs::exists(VAR_TO_STR(path))) {
        throw std::runtime_error("FileExistsError");
    }
    fs::create_directories(VAR_TO_STR(path));
    return NONE;
}

Variable os::readlink(Variable path) {
    auto p = VAR_TO_STR(path);
    if (fs::is_symlink(p)) {
        return NEW_STRING(fs::read_symlink(p));
    }
    throw std::runtime_error("Not a symlink");
}

Variable os::remove(Variable path) {
    auto p = VAR_TO_STR(path);
    if (!fs::exists(p)) {
        RAISE(FileNotFoundError, p + " does not exist");
    }
    if (fs::is_directory(p)) {
        RAISE(OSError, p + " is a directory");
    }
    fs::remove(p);
    return NONE;
}

Variable os::rmdir(Variable path) {
    fs::remove(VAR_TO_STR(path));
    return NONE;
}

Variable os::removedirs(Variable path) {
    fs::removeall(VAR_TO_STR(path));
    return NONE;
}

Variable os::rename(Variable src, Variable dst) {
    std::rename(VAR_TO_STR(src).c_str(), VAR_TO_STR(dst).c_str());
    return NONE;
}

Variable os::system(Variable cmd) {
    int res = std::system(VAR_TO_STR(cmd).c_str());
    return NEW_INT(res);
}

std::srand(std::time(nullptr));

Variable os::urandom(Variable size) {
    std::string res;
    for (size_t i = 0; i < VAR_TO_INT(size); ++i) {
        res += char(rand());
    }
    return NEW_STRING(res);
}

} // namespace MiniPython
