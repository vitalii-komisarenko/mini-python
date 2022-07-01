#pragma once

#include "token/Token.h"
#include "variable/Variable.h"

#include <memory>
#include <vector>

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
    TOKEN,
};

class Instruction {
public:
    Instruction();
    Instruction(Operation _op, std::vector<std::shared_ptr<Instruction>> _instructions);
    Instruction(const Token &_token);
    static Instruction fromTokenList(const TokenList &tokens);
    static Instruction fromTokenRange(std::vector<Token>::const_iterator &current, std::vector<Token>::const_iterator &end, TokenType endToken);

    Variable execute();
    Operation op = Operation::NONE;
    std::vector<std::shared_ptr<Instruction>> params;

    Variable var;
    Token token;
};

} // namespace MiniPython
