#pragma once

#include <memory>
#include <string>

namespace MiniPython {

enum class VariableType {
    NONE,
    INT,
    BOOL,
    FLOAT,
    STRING,
    LIST,
    DICT,
};

class GenericVariable;

using Variable = std::shared_ptr<GenericVariable>;

class GenericVariable {
public:
    virtual VariableType get_type() = 0;

    virtual Variable add(const Variable &other) = 0;
    virtual Variable sub(const Variable &other) = 0;
    virtual Variable mul(const Variable &other) = 0;
    virtual Variable div(const Variable &other) = 0;
    virtual Variable mod(const Variable &other) = 0;
    virtual Variable pow(const Variable &other) = 0;

    virtual bool to_bool() = 0;
    virtual std::string to_str() = 0;

    virtual bool equal(const Variable &other) = 0;
    virtual bool less(const Variable &other) = 0;
};

class NoneVariable: GenericVariable {
public:
    VariableType get_type() override;

    Variable add(const Variable &other) override;
    Variable sub(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable div(const Variable &other) override;
    Variable mod(const Variable &other) override;
    Variable pow(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;
};

class IntVariable: public GenericVariable {
public:
    using IntType = int64_t;

    IntVariable(IntType _value);

    VariableType get_type() override;

    Variable add(const Variable &other) override;
    Variable sub(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable div(const Variable &other) override;
    Variable mod(const Variable &other) override;
    Variable pow(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

private:
    IntType value;

    friend class StringVariable;
};

class BoolVariable: public GenericVariable {
public:
    BoolVariable(bool _value);

    VariableType get_type() override;

    Variable add(const Variable &other) override;
    Variable sub(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable div(const Variable &other) override;
    Variable mod(const Variable &other) override;
    Variable pow(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    Variable toIntVar();
private:
    bool value;
};

class StringVariable: public GenericVariable {
public:
    using StringType = std::string;

    StringVariable(StringType _value);

    VariableType get_type() override;

    Variable add(const Variable &other) override;
    Variable sub(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable div(const Variable &other) override;
    Variable mod(const Variable &other) override;
    Variable pow(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;
private:
    StringType value;
};

} // namespace MiniPython
