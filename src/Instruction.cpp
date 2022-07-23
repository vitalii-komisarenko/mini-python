#include "Instruction.h"
#include "TokenToVariable.h"

#include <stdexcept>

namespace MiniPython {

std::string opToString(Operation op) {
    switch (op) {
    case Operation::NONE:
        return "NONE";
    case Operation::ASSIGN:
        return "ASSIGN";
    case Operation::ADD:
        return "ADD";
    case Operation::SUB:
        return "SUB";
    case Operation::MUL:
        return "MUL";
    case Operation::DIV:
        return "DIV";
    case Operation::INT_DIV:
        return "INT_DIV";
    case Operation::MOD:
        return "MOD";
    case Operation::POW:
        return "POW";
    case Operation::CALL:
        return "CALL";
    case Operation::VAR_NAME:
        return "VAR_NAME";
    case Operation::RET_VALUE:
        return "RET_VALUE";
    case Operation::TOKEN:
        return "TOKEN";
    case Operation::IN_ROUND_BRACKETS:
        return "IN_ROUND_BRACKETS";
    case Operation::IN_SQUARE_BRACKETS:
        return "IN_SQUARE_BRACKETS";
    case Operation::IN_CURLY_BRACKETS:
        return "IN_CURLY_BRACKETS";
    default:
        return "????";
    }
}

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

#define CHECK_PARAM_SIZE(EXPECTED_SIZE) \
    if (params.size() != EXPECTED_SIZE) { \
        std::string error = "Wrong number of parameters for operation "; \
        error += opToString(op) + ": "; \
        error += std::to_string(EXPECTED_SIZE) + " expected but "; \
        error += std::to_string(params.size()) + " found."; \
        throw std::runtime_error(error); \
    }

Variable Instruction::execute() {
    switch(op) {
    case Operation::ADD: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute()->add(params[1]->execute());
    }
    case Operation::SUB: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute()->sub(params[1]->execute());
    }
    case Operation::MUL: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute()->mul(params[1]->execute());
    }
    case Operation::DIV: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute()->div(params[1]->execute());
    }
    case Operation::INT_DIV: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute()->int_div(params[1]->execute());
    }
    case Operation::MOD: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute()->mod(params[1]->execute());
    }
    case Operation::POW: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute()->pow(params[1]->execute());
    }
    case Operation::RET_VALUE: {
        CHECK_PARAM_SIZE(0);
        return var;
    }
    }
    return std::make_shared<NoneVariable>();
}

Instruction Instruction::fromTokenList(const TokenList &tokens) {
    auto begin = tokens.begin();
    auto end   = tokens.end();
    return fromTokenRange(begin, end, TokenType::NONE);
}

Instruction Instruction::fromTokenRange(std::vector<Token>::const_iterator &current, std::vector<Token>::const_iterator &end, TokenType endToken) {
    Instruction result;

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
            result.params[result.params.size() - 1]->op = Operation::IN_ROUND_BRACKETS;
            break;
        }
        case TokenType::OPENING_SQUARE_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<Instruction>(fromTokenRange(current, end, TokenType::CLOSING_SQUARE_BRACKET)));
            result.params[result.params.size() - 1]->op = Operation::IN_SQUARE_BRACKETS;
            break;
        }
        case TokenType::OPENING_CURLY_BRACKET: {
            ++current;
            result.params.push_back(std::make_shared<Instruction>(fromTokenRange(current, end, TokenType::CLOSING_CURLY_BRACKET)));
            result.params[result.params.size() - 1]->op = Operation::IN_CURLY_BRACKETS;
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

            for (size_t i = 1; i + 1 < result.params.size(); ++i) {
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
    groupByOperator({{"=", Operation::ASSIGN}});

    if (result.op == Operation::NONE && result.params.size() == 1) {
        result = *result.params[0].get();
    }

    if (result.op == Operation::NONE && result.params.size() == 2 && result.params[0]->op == Operation::VAR_NAME && result.params[1]->op == Operation::IN_ROUND_BRACKETS) {
        // TODO: a call to a function with more than 1 arg
        result.op = Operation::CALL;
        result.params[1] = result.params[1]->params[0];
    }

    return result;
}


} // namespace MiniPython
