#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

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
    FILE,
};

#define NONE std::make_shared<NoneVariable>()

class GenericVariable;

using Variable = std::shared_ptr<GenericVariable>;
using ListType = std::vector<Variable>;

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

    std::unordered_map<std::string, Variable> attr;
    Variable get_attr(const std::string &name);
    void set_attr(const std::string &name, Variable attr_value);
    bool has_attr(const std::string &name);

    // for test purposes
    virtual bool strictly_equal(const Variable &other) = 0;
};

class IterableVariable: public GenericVariable {
public:
    virtual ListType to_list() = 0;
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

class StringVariable: public IterableVariable {
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
    ListType to_list() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    StringType value;
};

class ListVariable: public IterableVariable {
public:
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
    ListType to_list() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    ListType list;
};

class Instruction;
class Scope;
using InstructionParams = std::vector<std::shared_ptr<Instruction>>;

using FunctionType = Variable(const InstructionParams&, Scope *scope);

class FunctionVariable: public GenericVariable {
public:
    FunctionVariable(FunctionType& function);

    VariableType get_type() override;
    FunctionType& get_value();
    Variable call(const InstructionParams &params, Scope *scope);

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
