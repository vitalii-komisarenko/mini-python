#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace MiniPython {

enum class VariableType {
    NONE,
    OBJECT_NOT_FOUND,
    INT,
    BOOL,
    FLOAT,
    COMPLEX,
    STRING,
    BYTES,
    LIST,
    ARRAY,
    SET,
    DICT,
    FUNCTION,
    FILE,
    MODULE,
};

#define NEW_BOOL(value) ((value) ? TRUE : FALSE)
#define NEW_INT(value) std::make_shared<MiniPython::IntVariable>(value)
#define NEW_FLOAT(value) std::make_shared<MiniPython::FloatVariable>(value)
#define NEW_STRING(str) std::make_shared<StringVariable>(str)
#define NEW_BYTES(str) std::make_shared<Bytes>(str)
#define NEW_LIST(list) std::make_shared<ListVariable>(list)
#define NEW_SET(set) std::make_shared<SetVariable>(set)

#define VAR_TO_BOOL(var) std::dynamic_pointer_cast<BoolVariable>(var)->value
#define VAR_TO_INT(var) std::dynamic_pointer_cast<IntVariable>(var)->value
#define VAR_TO_FLOAT(var) std::dynamic_pointer_cast<FloatVariable>(var)->value
#define VAR_TO_STR(var) ((var->get_type() == VariableType::BYTES) ? VAR_TO_BYTES(var) : std::dynamic_pointer_cast<StringVariable>(var)->value)
#define VAR_TO_BYTES(var) std::dynamic_pointer_cast<Bytes>(var)->value
#define VAR_TO_LIST(var) std::dynamic_pointer_cast<ListVariable>(var)->list

class GenericVariable;

using Variable = std::shared_ptr<GenericVariable>;
using IntType = int64_t;
using ListType = std::vector<Variable>;
using FloatType = double;

class GenericVariable {
public:
    virtual VariableType get_type() = 0;
    std::string get_class_name();

    virtual Variable add(const Variable &other);
    virtual Variable sub(const Variable &other);
    virtual Variable mul(const Variable &other);
    virtual Variable div(const Variable &other);
    virtual Variable mod(const Variable &other);
    virtual Variable pow(const Variable &other);
    virtual Variable int_div(const Variable &other);

    virtual bool to_bool();
    virtual IntType to_int();
    Variable to_int_var();
    virtual std::string to_str();

    virtual Variable to_bytes_variable();

    virtual bool equal(const Variable &other);
    virtual bool less(const Variable &other);

    std::unordered_map<std::string, Variable> attr;
    Variable get_attr(const std::string &name);
    void set_attr(const std::string &name, Variable attr_value);
    bool has_attr(const std::string &name);

    // for test purposes
    virtual bool strictly_equal(const Variable &other);
};

class IterableVariable: public GenericVariable {
public:
    virtual ListType to_list() = 0;
    bool contains(const Variable &item);
};

class NoneVariable: public GenericVariable {
public:
    VariableType get_type() override;

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool strictly_equal(const Variable &other) override;
};

static Variable NONE = std::make_shared<NoneVariable>();

class ObjectNotFoundVariable: public GenericVariable {
public:
    VariableType get_type() override;
    bool equal(const Variable &other) override;
};

static Variable OBJECT_NOT_FOUND = std::make_shared<ObjectNotFoundVariable>();

class IntVariable: public GenericVariable {
public:
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
    IntType to_int() override;
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
    IntType to_int() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    Variable toIntVar();
    Variable toFloatVar();

    bool value;
};

static auto TRUE = std::make_shared<BoolVariable>(true);
static auto FALSE = std::make_shared<BoolVariable>(false);

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

    FloatType value;
};

class ComplexVariable: public GenericVariable {
public:
    ComplexVariable(FloatType _real, FloatType _imag = 0);

    VariableType get_type() override;

    Variable add(const Variable &other) override;
    Variable sub(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable div(const Variable &other) override;
    Variable pow(const Variable &other) override;

    FloatType abs();
    Variable conjugate();

    bool to_bool() override;
    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    FloatType real;
    FloatType imag;
};

class StringVariable: public IterableVariable {
public:
    using StringType = std::string;

    StringVariable(const StringType &_value);

    VariableType get_type() override;
    StringType get_value();

    Variable add(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable mod(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;
    ListType to_list() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    StringType value;
};

class Bytes: public StringVariable {
public:
    Bytes(const StringType &_value);

    VariableType get_type() override;
    StringType get_value();

    Variable add(const Variable &other) override;
    Variable mul(const Variable &other) override;
    Variable mod(const Variable &other) override;

    std::string to_str() override;
    Variable to_bytes_variable() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;
};

/**
 * @brief list and tuple representation
 *
 * They differ only by is_tuple flag.
 */
class ListVariable: public IterableVariable {
public:
    ListVariable();
    ListVariable(ListType _list);

    VariableType get_type() override;
    ListType get_value();

    Variable add(const Variable &other) override;
    Variable mul(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;
    ListType to_list() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    ListType list;
    bool is_tuple;
};

bool is_tuple(Variable var);

class ArrayVariable: public ListVariable {
public:
    ArrayVariable(Variable typecode, ListType _list);

    VariableType get_type() override { return VariableType::ARRAY; }
    std::string to_str() override;
};

class SetVariable: public IterableVariable {
public:
    SetVariable();
    SetVariable(ListType *_list);
    SetVariable(IterableVariable *_list);

    VariableType get_type() override;
    ListType get_value();

    Variable sub(const Variable &other) override;

    bool to_bool() override;
    std::string to_str() override;
    ListType to_list() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    ListType list;
};

class DictVariable: public IterableVariable {
public:
    DictVariable();
    DictVariable(const Variable &keys, const Variable value); // fromkeys()

    VariableType get_type() override;

    ListType keys();
    ListType values();
    Variable get_item(Variable key); // [] operator
    Variable set_item(Variable key, Variable value);
    Variable get(Variable key, Variable default_value); // get()
    Variable clear();
    Variable copy();
    Variable pop(Variable key);
    Variable popitem();

    bool to_bool() override;
    std::string to_str() override;
    ListType to_list() override;

    bool equal(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    std::vector<std::pair<Variable, Variable>> pairs;
private:
    /**
     * @brief a helper function for [] operator and get() method
     *
     * @return value correspoding the key if exists or OBJECT_NOT_FOUND otherwise
     */
    Variable get_item_helper(Variable key);
    /**
     * @brief a helper function for del statement and pop() method
     *
     * @return value correspoding the key if exists or OBJECT_NOT_FOUND otherwise
     */
    Variable del_item_helper(Variable key);
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
    Variable call(Variable &param, Scope *scope);
    Variable call(Variable &param1, Variable &param2, Scope *scope);

    std::string to_str() override;

    bool equal(const Variable &other) override;
    bool less(const Variable &other) override;

    bool strictly_equal(const Variable &other) override;

    void append(const Variable &other);
private:
    FunctionType& value;
};

class ModuleVariable: public GenericVariable {
public:
    VariableType get_type() override { return VariableType::MODULE; }
    std::string to_str() override { return "<module>"; }
};

} // namespace MiniPython
