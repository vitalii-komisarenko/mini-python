#include "Instruction.h"
#include "token2variable/TokenToVariable.h"

#include <stdexcept>

namespace MiniPython {

Instruction::Instruction() {}

Instruction::Instruction(Operation _op, std::vector<std::shared_ptr<Instruction>> _params)
    : op(_op)
    , params(_params)
    {}

Instruction::Instruction(const Token &_token)
{
    switch (_token.type) {
    case TokenType::IDENTIFIER: {
        op = Operation::VAR_NAME;
        var = std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>(_token.value));
        break;
    }
    case TokenType::NUMBER:
    case TokenType::STRING: {
        op = Operation::RET_VALUE;
        var = parseTokenToVariable(_token);
        break;
    }
    default: {
        op = Operation::TOKEN;
        token = _token;
    }
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
            result.params.push_back(std::make_shared<Instruction>(*current));
            ++current;
            return result;
        }

        switch (current->type) {
        case TokenType::OPENING_ROUND_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<Instruction>(fromTokenRange(current, end, TokenType::CLOSING_ROUND_BRACKET)));
            break;
        }
        case TokenType::OPENING_SQUARE_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<Instruction>(fromTokenRange(current, end, TokenType::CLOSING_SQUARE_BRACKET)));
            break;
        }
        case TokenType::OPENING_CURLY_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<Instruction>(fromTokenRange(current, end, TokenType::CLOSING_CURLY_BRACKET)));
            break;
        }
        default: {
            result.params.push_back(std::make_shared<Instruction>(Token(*current)));
            ++current;
        }
        }
    }

    auto groupByOperator = [&result](std::vector<std::pair<const char *, Operation>> ops) {// *op_str, Operation target_op) {
        auto canBeArithmeticOperand = [](std::shared_ptr<Instruction> &instr) {
//            return instr->op == Operation::VAR_NAME
//                || instr->op == Operation::RET_VALUE;
            return instr->op != Operation::TOKEN;
        };

        bool found = true;
        while (found) {
            found = false;

            for (size_t i = 1; i < result.params.size() - 1; ++i) {
                if (canBeArithmeticOperand(result.params[i - 1])
                        && canBeArithmeticOperand(result.params[i + 1])
                        && result.params[i]->token.type == TokenType::OPERATOR) {

                    for (const auto& pair: ops) {
                        if (result.params[i]->token.value == pair.first) {
                            std::shared_ptr<Instruction> instr = std::make_shared<Instruction>();
                            instr->op = pair.second;
                            instr->params.push_back(result.params[i - 1]);
                            instr->params.push_back(result.params[i + 1]);

                            result.params.erase(result.params.begin() + i - 1, result.params.begin() + i + 2);
                            result.params.insert(result.params.begin() + i - 1, instr);

                            found = true;
                            goto end_while;
                        }
                    }
                }
            }

            end_while:
            ;
        }
    };

    groupByOperator({{"**", Operation::POW}});
    groupByOperator({{"*", Operation::MUL},
                     {"/", Operation::DIV},
                     {"//", Operation::INT_DIV},
                     {"%", Operation::MOD}});
    groupByOperator({{"+", Operation::ADD},
                     {"-", Operation::SUB}});
    groupByOperator({{"=", Operation::SET}});

    if (result.op == Operation::NONE && result.params.size() == 1) {
        result = *result.params[0].get();
    }

    return result;
}


} // namespace MiniPython
