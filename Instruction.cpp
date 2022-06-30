#include "Instruction.h"

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
            //result.params.push_back(std::make_shared<InstructionOrVariable>(Token(current->type, current->value)));
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

    return result;
}


} // namespace MiniPython
