#include "Instruction.h"
#include "token2variable/TokenToVariable.h"

#include <stdexcept>

namespace MiniPython {

Instruction::Instruction(Operation _op, std::vector<Instruction> instructions) {
    op = _op;
    for (auto& i: instructions) {
        std::shared_ptr<InstructionOrVariable> iov = std::make_shared<InstructionOrVariable>();
        iov->type = InstructionOrVariableType::INSTRUCTION;
        iov->instruction = i;
        params.push_back(iov);
    }
}

InstructionOrVariable::InstructionOrVariable()
    : type(InstructionOrVariableType::NONE)
    , token(TokenType::NONE, "")
    {}

InstructionOrVariable::InstructionOrVariable(const Token &_token)
    : type(InstructionOrVariableType::UNPARSED_TOKEN)
    , token(_token)
    {}

InstructionOrVariable::InstructionOrVariable(const Instruction &_instruction)
    : type(InstructionOrVariableType::INSTRUCTION)
    , instruction(_instruction)
    , token(TokenType::NONE, "")
    {}

Variable InstructionOrVariable::execute() {
    switch (type) {
    case InstructionOrVariableType::INSTRUCTION:
        return instruction.execute();
    case InstructionOrVariableType::VARIABLE:
        return variable;
    default:
        throw std::runtime_error("Bad InstructionOrVariable type");
    }
}

Variable Instruction::execute() {
    return std::make_shared<NoneVariable>();
}

Instruction Instruction::fromTokenList(const TokenList &tokens) {
    auto begin = tokens.begin();
    auto end   = tokens.end();
    return fromTokenRange(begin, end, TokenType::NONE);
}

Instruction Instruction::fromTokenRange(std::vector<Token>::const_iterator &_current, std::vector<Token>::const_iterator &end, TokenType endToken) {
    Instruction result;

    auto current = _current;

    while (current != end) {
        if (current->type == endToken) {
            result.params.push_back(std::make_shared<InstructionOrVariable>(*current));
            ++current;
            return result;
        }

        switch (current->type) {
        case TokenType::OPENING_ROUND_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<InstructionOrVariable>(fromTokenRange(current, end, TokenType::CLOSING_ROUND_BRACKET)));
            break;
        }
        case TokenType::OPENING_SQUARE_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<InstructionOrVariable>(fromTokenRange(current, end, TokenType::CLOSING_SQUARE_BRACKET)));
            break;
        }
        case TokenType::OPENING_CURLY_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<InstructionOrVariable>(fromTokenRange(current, end, TokenType::CLOSING_CURLY_BRACKET)));
            break;
        }
        default: {
            result.params.push_back(std::make_shared<InstructionOrVariable>(Token(*current)));
            ++current;
        }
        }
    }

    auto canBeArithmeticOperand = [](std::shared_ptr<InstructionOrVariable> &iov) {
        if (iov->type == InstructionOrVariableType::INSTRUCTION)
            return true;

        if (iov->type == InstructionOrVariableType::VARIABLE)
            return true;

        if (iov->type == InstructionOrVariableType::UNPARSED_TOKEN) {
            if (iov->token.type == TokenType::IDENTIFIER)
                return true;
            if (iov->token.type == TokenType::NUMBER)
                return true;
            if (iov->token.type == TokenType::STRING)
                return true;
        }

        return false;
    };

    auto fromArithmeticOperandToken = [](const Token &token) {
        Instruction result;
        switch (token.type) {
        case TokenType::IDENTIFIER: {
            result.op = Operation::VAR_NAME;
            result.var = std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>(token.value));
            break;
        }
        case TokenType::NUMBER:
        case TokenType::STRING: {
            result.op = Operation::RET_VALUE;
            result.var = parseTokenToVariable(token);
            break;
        }
        default: {
            throw std::runtime_error("Can't turn this token into a single instruction");
        }
        }
        return result;
    };

    // =

    bool found = true;
    while (found) {
        found = false;

        for (size_t i = 1; i < result.params.size() - 1; ++i) {
            if (canBeArithmeticOperand(result.params[i - 1])
                    && canBeArithmeticOperand(result.params[i + 1])
                    && result.params[i]->token.type == TokenType::OPERATOR
                    && result.params[i]->token.value == "=") {

                std::shared_ptr<InstructionOrVariable> iov = std::make_shared<InstructionOrVariable>();
                iov->type = InstructionOrVariableType::INSTRUCTION;
                iov->instruction.op = Operation::SET;
                iov->instruction.params.push_back(result.params[i - 1]);
                iov->instruction.params.push_back(result.params[i + 1]);

                result.params.erase(result.params.begin() + i - 1, result.params.begin() + i + 2);
                result.params.insert(result.params.begin() + i - 1, iov);

                found = true;
            }
        }
    }

    if (result.params.size() == 1 && result.op == Operation::NONE && result.params[0]->type == InstructionOrVariableType::INSTRUCTION) {
        result = result.params[0]->instruction;
    }

    if (result.params.size() == 1 && result.op == Operation::NONE && result.params[0]->type == InstructionOrVariableType::UNPARSED_TOKEN) {
        if (result.params.size() == 1 && canBeArithmeticOperand(result.params[0])) {
            result = fromArithmeticOperandToken(result.params[0]->token);
        }
    }

    return result;
}


} // namespace MiniPython
