#pragma once

#include "Variable.h"

namespace MiniPython {

class FileVariable: public GenericVariable {
public:
    FileVariable(const std::string &filename, const std::string &mode);
    ~FileVariable();

    VariableType get_type() override;

    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    FILE *fh;
};

} //namespace MiniPython
