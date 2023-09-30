#pragma once

#include "../variable/Variable.h"

namespace MiniPython {

class os {
public:
    std::shared_ptr<DictVariable> env;
    Variable getenv(Variable key, Variable default_value = NONE);
    Variable putenv(Variable key, Variable value);
    Variable unsetenv(Variable key);
    Variable getcwd();
    Variable chdir(Variable path);
    Variable listdir(Variable path = NEW_STRING("."));
    Variable mkdir(Variable path, Variable mode = NEW_INT(0777));
    Variable makedirs(Variable path, Variable mode = NEW_INT(0777), Variable exists_ok = NEW_BOOL(false));
    Variable readlink(Variable path);
    Variable remove(Variable path);
    Variable rmdir(Variable path);
    Variable removedirs(Variable path);
    Variable rename(Variable src, Variable dst);
    Variable system(Variable cmd);
    Variable urandom(Variable size);

#ifdef _WIN32
    static Variable devnull = NEW_STRING("nul");
    static Variable sep = NEW_STRING("\\");
    static Variable linesep = NEW_STRING("\r\n");
    static Variable pathsep = NEW_STRING(";");
#else
    static Variable devnull = NEW_STRING("/dev/null");
    static Variable sep = NEW_STRING("/");
    static Variable linesep = NEW_STRING("\n");
    static Variable pathsep = NEW_STRING(":");
#endif

private:
    void make_environ();
};

} // namespace MiniPython
