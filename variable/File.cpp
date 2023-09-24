#include "File.h"

#include <stdexcept>

namespace MiniPython {

extern Variable execute_instruction(std::shared_ptr<Instruction> instr, Scope *scope);

#define VAR(i) execute_instruction(params[i], scope)
#define INT(i) std::dynamic_pointer_cast<IntVariable>(VAR(i))
#define STRING(i) std::dynamic_pointer_cast<StringVariable>(VAR(i))
#define FILE_VAR(i) std::dynamic_pointer_cast<FileVariable>(VAR(i))

#define NEW_INT(value) std::make_shared<MiniPython::IntVariable>(value)
#define NEW_STRING(value) std::make_shared<MiniPython::StringVariable>(value)

static Variable read_str(FILE *fh, size_t size) {
    std::string res(size, 0);
    int ret = fread(&res[0], 1, size, fh);
    if (ret < 0) {
        throw std::runtime_error("fread error");
    }
    res.resize(ret);
    return NEW_STRING(res);
}

static Variable read(const InstructionParams& params, Scope *scope) {
    auto fh = FILE_VAR(0)->fh;
    if (params.size() == 1) {
        // read whole file
        auto current_pos = ftell(fh);
        fseek(fh, 0, SEEK_END);
        auto end_pos = ftell(fh);
        fseek(fh, current_pos, SEEK_SET);

        auto size = end_pos - current_pos;
        return read_str(fh, size);
    }

    // read exact number of bytes
    return read_str(fh, INT(0)->value);
}

static Variable write(const InstructionParams& params, Scope *scope) {
    auto &str = STRING(1)->value;
    return NEW_INT(fwrite(str.c_str(), 1, str.size(), FILE_VAR(0)->fh));
}

static Variable readline(const InstructionParams& params, Scope *scope) {
    char *line = NULL;
    size_t len = 0;
    ssize_t ret = getline(&line, &len, FILE_VAR(0)->fh);
    if (ret == -1) {
        return NEW_STRING("");
    }
    std::string res(line, len);
    delete line;
    return NEW_STRING(res);
}

static Variable readlines(const InstructionParams& params, Scope *scope) {
    auto list = std::make_shared<ListVariable>();
    while(true) {
        auto line_var = readline(params, scope);
        if (std::dynamic_pointer_cast<StringVariable>(line_var)->value == "") {
            break;
        }
        list->list.push_back(line_var);
    }
    return list;
}

static Variable tell(const InstructionParams& params, Scope *scope) {
    return NEW_INT(ftell(FILE_VAR(0)->fh));
}

static Variable flush(const InstructionParams& params, Scope *scope) {
    fflush(FILE_VAR(0)->fh);
    return NONE;
}

static Variable seek(const InstructionParams& params, Scope *scope) {
    int whence = SEEK_SET;
    if (params.size() == 3) {
        switch (INT(2)->value) {
        case 0: whence = SEEK_SET; break;
        case 1: whence = SEEK_CUR; break;
        case 2: whence = SEEK_END; break;
        default: throw std::runtime_error("seek: Unsupported value of whence");
        }
    }
    return NEW_INT(fseek(FILE_VAR(0)->fh, INT(1)->value, whence));
}

FileVariable::FileVariable(const std::string &filename, const std::string &mode) :
    fh(fopen(filename.c_str(), mode.c_str())) {}

FileVariable::~FileVariable() {
    fclose(fh);
}

VariableType FileVariable::get_type() {
    return VariableType::FILE;
}

Variable FileVariable::add(const Variable &other) { throw std::runtime_error("FileVariable: add not supported"); }
Variable FileVariable::sub(const Variable &other) { throw std::runtime_error("FileVariable: sub not supported"); }
Variable FileVariable::mul(const Variable &other) { throw std::runtime_error("FileVariable: mul not supported"); }
Variable FileVariable::div(const Variable &other) { throw std::runtime_error("FileVariable: div not supported"); }
Variable FileVariable::mod(const Variable &other) { throw std::runtime_error("FileVariable: mod not supported"); }
Variable FileVariable::pow(const Variable &other) { throw std::runtime_error("FileVariable: pow not supported"); }
Variable FileVariable::int_div(const Variable &other) { throw std::runtime_error("FileVariable: int_div not supported"); }

bool FileVariable::to_bool() {
    return true;

}
std::string FileVariable::to_str() {
    return "<FileVariable>";
}

bool FileVariable::equal(const Variable &other) {
    return false;
}
bool FileVariable::less(const Variable &other) {
    throw std::runtime_error("Comparison of file objects not supported");
}

bool FileVariable::strictly_equal(const Variable &other) { throw std::runtime_error("FileVariable: strictly_equal not supported"); }

} // namespace MiniPython
