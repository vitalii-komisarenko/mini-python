#include "Instruction.h"
#include "Scope.h"
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

Instruction::Instruction(): op(Operation::NONE) {}

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

Variable execute_instruction(Instruction *instr, Scope *scope) {
    return instr->execute(scope);
}

Variable Instruction::execute(Scope *scope) {
    switch(op) {
    case Operation::ASSIGN: {
        CHECK_PARAM_SIZE(2);
        auto var_name = params[0]->var->to_str();
        auto scope_with_variable = scope->scopeWithVariable(var_name);
        if (!scope_with_variable) {
            scope_with_variable = scope->parentScope.lock()->impl;
        }
        scope_with_variable->vars.set(var_name, params[1]->execute(scope));
        return nullptr; // TODO
    }
    case Operation::ADD: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute(scope)->add(params[1]->execute(scope));
    }
    case Operation::SUB: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute(scope)->sub(params[1]->execute(scope));
    }
    case Operation::MUL: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute(scope)->mul(params[1]->execute(scope));
    }
    case Operation::DIV: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute(scope)->div(params[1]->execute(scope));
    }
    case Operation::INT_DIV: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute(scope)->int_div(params[1]->execute(scope));
    }
    case Operation::MOD: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute(scope)->mod(params[1]->execute(scope));
    }
    case Operation::POW: {
        CHECK_PARAM_SIZE(2);
        return params[0]->execute(scope)->pow(params[1]->execute(scope));
    }
    case Operation::VAR_NAME: {
        CHECK_PARAM_SIZE(0);
        if (!scope) {
            throw std::runtime_error("instruction: scope already destroyed");
        }
        return scope->getVariable(var->to_str());
    }
    case Operation::RET_VALUE: {
        CHECK_PARAM_SIZE(0);
        return var;
    }
    case Operation::CALL: {
        CHECK_PARAM_SIZE(2);
        InstructionParams call_args;
        call_args.push_back(params[1]);
        auto func = std::dynamic_pointer_cast<FunctionVariable>(params[0]->execute(scope));
        return func->call((const InstructionParams)(params[1]->params), scope);
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
            ++current;
            break;
        }

        switch (current->type) {
        case TokenType::OPENING_ROUND_BRACKET: {
            ++current;
            auto in_round_brackets = std::make_shared<Instruction>(fromTokenRange(current, end, TokenType::CLOSING_ROUND_BRACKET));
            std::vector<std::shared_ptr<Instruction>> params = {in_round_brackets};
            result.params.push_back(std::make_shared<Instruction>(Operation::IN_ROUND_BRACKETS, params));
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

    auto groupByOperator = [&result](std::vector<std::pair<const char *, Operation>> ops) {
        auto canBeArithmeticOperand = [](std::shared_ptr<Instruction> &instr) {
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
        Instruction tmp = result; // TODO: it prevents InstructionTest from corrupting memory. Check what is the root cause
        result = *result.params[0].get();
    }

    if (result.op == Operation::NONE && result.params.size() == 2 && result.params[0]->op == Operation::VAR_NAME && result.params[1]->op == Operation::IN_ROUND_BRACKETS) {
        // TODO: a call to a function with more than 1 arg
        result.op = Operation::CALL;
    }

    if ((result.op == Operation::CALL) && (result.params.size() == 2) && (result.params[1]->op == Operation::IN_ROUND_BRACKETS) && (result.params[1]->params.size() == 1) && (result.params[1]->params[0]->op == Operation::NONE)) {
        result.params[1] = result.params[1]->params[0];
        result.params[1]->op = Operation::IN_ROUND_BRACKETS;

        // Transform param1, <comma>, param2, <comma>, param3, ..., <comma>, paramN --> param1, param2, ..., paramN
        bool ok = true;
        for (size_t i = 0; i < result.params[1]->params.size(); ++i) {
            bool is_comma = result.params[1]->params[i]->op == Operation::TOKEN && result.params[1]->params[i]->token.type == TokenType::COMMA; //value == ",";
            if ((i % 2) != is_comma) {
                ok = false;
            }
        }
        if (ok) {
            for (size_t i = result.params[1]->params.size() / 2; i > 0; --i) {
                result.params[1]->params.erase(result.params[1]->params.begin() + 2*i-1);
            }
        }
    }

    return result;
}

std::string Instruction::debug_string(int indent_level) {
    std::string result = "";
    for (size_t i = 0; i < indent_level; ++i) {
        result += "  ";
    }
    result += opToString(op) + "\n";
    for (auto param: params) {
        result += param->debug_string(indent_level + 1);
    }
    return result;
}

} // namespace MiniPython
