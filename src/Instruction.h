#pragma once

#include "Token.h"
#include "Variable.h"

#include <memory>
#include <string>
#include <vector>

namespace MiniPython {

class Instruction;
using InstructionParams = std::vector<std::shared_ptr<Instruction>>;

enum class Operation {
    NONE,
    ASSIGN,
    ADD,
    SUB,
    MUL,
    DIV,
    INT_DIV,
    MOD,
    POW,
    CALL,
    VAR_NAME,
    RET_VALUE,
    // temporary tokens during parsing
    TOKEN,
    IN_ROUND_BRACKETS,
    IN_SQUARE_BRACKETS,
    IN_CURLY_BRACKETS,
};

std::string opToString(Operation op);

class Instruction {
public:
    Instruction();
    Instruction(Operation _op, std::vector<std::shared_ptr<Instruction>> _instructions);
    Instruction(const Token &_token);
    static Instruction fromTokenList(const TokenList &tokens);
    static Instruction fromTokenRange(std::vector<Token>::const_iterator &current, std::vector<Token>::const_iterator &end, TokenType endToken);

    Variable execute();
    Operation op = Operation::NONE;
    InstructionParams params;

    Variable var;
    Token token;
};

} // namespace MiniPython
