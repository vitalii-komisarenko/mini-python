#pragma once

#include <memory>
#include <string>
#include <vector>

namespace MiniPython {

enum class VariableType {
    NONE,
    INT,
    BOOL,
    FLOAT,
    STRING,
    LIST,
    DICT,
    FUNCTION,
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
    virtual Variable int_div(const Variable &other) = 0;

    virtual bool to_bool() = 0;
    virtual std::string to_str() = 0;

    virtual bool equal(const Variable &other) = 0;
    virtual bool less(const Variable &other) = 0;

    // for test purposes
    virtual bool strictly_equal(const Variable &other) = 0;
};

class NoneVariable: public GenericVariable {
public:
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
};

class IntVariable: public GenericVariable {
public:
    using IntType = int64_t;

    IntVariable(IntType _value);

    VariableType get_type() override;
    IntType get_value();

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

    Variable toFloatVar();
private:
    IntType value;
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
    Variable int_div(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    Variable toIntVar();
    Variable toFloatVar();
private:
    bool value;
};

class FloatVariable: public GenericVariable {
public:
    using FloatType = double;

    FloatVariable(FloatType _value);

    VariableType get_type() override;
    FloatType get_value();

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

    Variable toFloatVar();
private:
    FloatType value;
};

class StringVariable: public GenericVariable {
public:
    using StringType = std::string;

    StringVariable(const StringType &_value);

    VariableType get_type() override;
    StringType get_value();

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
private:
    StringType value;
};

class ListVariable: public GenericVariable {
public:
    using ListType = std::vector<Variable>;

    ListVariable();
    ListVariable(ListType _list);

    VariableType get_type() override;
    ListType get_value();

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

    void append(const Variable &other);
private:
    ListType list;
};

class Instruction;
class Scope;
using InstructionParams = std::vector<std::shared_ptr<Instruction>>;

using FunctionType = Variable(const InstructionParams&, std::shared_ptr<Scope> scope);

class FunctionVariable: public GenericVariable {
public:
    FunctionVariable(FunctionType& function);

    VariableType get_type() override;
    FunctionType& get_value();
    Variable call(const InstructionParams &params, std::shared_ptr<Scope> scope);

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

    void append(const Variable &other);
private:
    FunctionType& value;
};
} // namespace MiniPython
