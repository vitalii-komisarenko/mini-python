#pragma once

#include "token/Token.h"
#include "variable/Variable.h"

#include <memory>

namespace MiniPython {

class Instruction;

enum class Operation {
    NONE,
    SET,
    ADD,
    SUB,
    MUL,
    DIV,
    INT_DIV,
    MOD,
    CALL,
    VAR_NAME,
    RET_VALUE,
    // temporary tokens during parsing
    TOKEN_SEQUENCE_AFTER_BRACKETS_PARSING,
};

struct InstructionOrVariable;

class Instruction {
public:
    Instruction() {};
    Instruction(Operation _op, std::vector<Instruction> instructions);
    static Instruction fromTokenList(const TokenList &tokens);
    static Instruction fromTokenRange(std::vector<Token>::const_iterator &current, std::vector<Token>::const_iterator &end, TokenType endToken);

    Variable execute();
    Operation op = Operation::NONE;
    std::vector<std::shared_ptr<InstructionOrVariable>> params;

    Variable var;
};

enum class InstructionOrVariableType {
    NONE,
    INSTRUCTION,
    VARIABLE,
    UNPARSED_TOKEN,
};

struct InstructionOrVariable {
    InstructionOrVariable();
    InstructionOrVariable(const Token &_token);
    InstructionOrVariable(const Instruction &_instruction);

    InstructionOrVariableType type;

    Instruction instruction;
    Variable variable;
    Token token;

    Variable execute();
};

} // namespace MiniPython
