#pragma once

#include "Variable.h"

namespace MiniPython {

class FileVariable: public GenericVariable {
public:
    FileVariable(const std::string &filename, const std::string &mode);
    ~FileVariable();

    VariableType get_type() override;

    Variable add(const Variable &other) override;
    Variable sub(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable div(const Variable &other) override;
    Variable mod(const Variable &other) override;
    Variable pow(const Variable &other) override;
    Variable int_div(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    FILE *fh;
};

} //namespace MiniPython
