#include "Module.h"
#include "Instruction.h"
#include "FunctionParamatersParsing.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <stdlib.h>
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

static Variable getenv(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"key", "default"},
        {{"default", NONE}}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(key);
    auto default_value = parsed_params.vars["default"];

    char *value = std::getenv(std::dynamic_pointer_cast<StringVariable>(key)->value.c_str());
    if (!value) {
        return default_value;
    }
    return std::make_shared<StringVariable>(value);
}

static Variable putenv(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"key", "value"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(key);
    PARSE_ARG(value);

    setenv(std::dynamic_pointer_cast<StringVariable>(key)->value.c_str(),
           std::dynamic_pointer_cast<StringVariable>(value)->value.c_str(),
           true);
    return NONE;
}

static Variable unsetenv(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"key", "value"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(key);

    ::unsetenv(std::dynamic_pointer_cast<StringVariable>(key)->value.c_str());
    return NONE;
}

static Variable getcwd(const InstructionParams &params, Scope *scope) {
    return std::make_shared<StringVariable>(std::filesystem::current_path());
}

static Variable chdir(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(path);

    std::filesystem::current_path(VAR_TO_STR(path));
    return NONE;
}

static Variable listdir(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path"},
        {{"path", NEW_STRING(".")}}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(path);

    std::vector<Variable> list;
    for (const auto & entry : std::filesystem::directory_iterator(VAR_TO_STR(path))) {
        if ((entry.path() != ".") && (entry.path() != "..")) {
            list.push_back(NEW_STRING(entry.path()));
        }
    }
    return NEW_LIST(list);
}

static Variable mkdir(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path", "mode"},
        {{"mode", NEW_INT(0777)}}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(path);
    PARSE_ARG(mode);

    if (fs::exists(VAR_TO_STR(path))) {
        throw std::runtime_error("FileExistsError");
    }
    fs::create_directory(VAR_TO_STR(path));
    return NONE;
}

static Variable makedirs(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path", "mode", "exists_ok"},
        {
            {"mode", NEW_INT(0777)},
            {"exists_ok", FALSE},
        }
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(path);
    PARSE_ARG(mode);
    PARSE_ARG(exists_ok);

    if (fs::exists(VAR_TO_STR(path))) {
        throw std::runtime_error("FileExistsError");
    }
    fs::create_directories(VAR_TO_STR(path));
    return NONE;
}

static Variable readlink(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);
    PARSE_ARG(path);

    auto p = VAR_TO_STR(path);
    if (fs::is_symlink(p)) {
        return NEW_STRING(fs::read_symlink(p));
    }
    throw std::runtime_error("Not a symlink");
}

static Variable remove(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(path);

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

static Variable rmdir(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(path);
    fs::remove(VAR_TO_STR(path));
    return NONE;
}

static Variable removedirs(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"path"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(path);
    fs::remove_all(VAR_TO_STR(path));
    return NONE;
}

static Variable rename(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"src", "dst"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);

    PARSE_ARG(src);
    PARSE_ARG(dst);

    std::rename(VAR_TO_STR(src).c_str(), VAR_TO_STR(dst).c_str());
    return NONE;
}

static Variable system(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"cmd"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);
    PARSE_ARG(cmd);

    int res = std::system(VAR_TO_STR(cmd).c_str());
    return NEW_INT(res);
}

static Variable urandom(const InstructionParams &params, Scope *scope) {
    FunctionParameterSchema schema = {
        {"cmd"},
        {}
    };

    auto parsed_params = ParsedFunctionParamaters::parse(params, scope, schema);
    PARSE_ARG(size);

    std::string res;
    for (size_t i = 0; i < VAR_TO_INT(size); ++i) {
        res += char(rand());
    }
    return NEW_STRING(res);
}

os::os() {
    SET_FUNCTION("getenv", getenv);
    SET_FUNCTION("putenv", putenv);
    SET_FUNCTION("unsetenv", unsetenv);
    SET_FUNCTION("getcwd", getcwd);
    SET_FUNCTION("chdir", chdir);
    SET_FUNCTION("listdir", listdir);
    SET_FUNCTION("mkdir", mkdir);
    SET_FUNCTION("makedirs", makedirs);
    SET_FUNCTION("readlink", readlink);
    SET_FUNCTION("remove", remove);
    SET_FUNCTION("rmdir", rmdir);
    SET_FUNCTION("removedirs", removedirs);
    SET_FUNCTION("rename", rename);
    SET_FUNCTION("system", system);
    SET_FUNCTION("urandom", urandom);

#ifdef _WIN32
    set_attr("devnull", NEW_STRING("nul"));
    set_attr("sep", NEW_STRING("\\"));
    set_attr("linesep", NEW_STRING("\r\n"));
    set_attr("pathsep", NEW_STRING(";"));
#else
    set_attr("devnull", NEW_STRING("/dev/null"));
    set_attr("sep", NEW_STRING("/"));
    set_attr("linesep", NEW_STRING("\n"));
    set_attr("pathsep", NEW_STRING(":"));
#endif

}

} // namespace MiniPython
