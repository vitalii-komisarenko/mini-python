#include "Instruction.h"
#include "Scope.h"
#include "StringFormatting.h"
#include "TokenToVariable.h"

#include <stdexcept>

namespace MiniPython {

std::string opToString(Operation op) {
    switch (op) {
    case Operation::NONE:
        return "NONE";
    case Operation::ASSIGN:
        return "ASSIGN";
    case Operation::KWARG:
        return "KWARG";
    case Operation::ATTR:
        return "ATTR";
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
    case Operation::FSTRING:
        return "FSTRING";
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

Instruction::Instruction(Variable _var)
    : op(Operation::RET_VALUE)
    , var(_var)
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
    case TokenType::STRING:
    case TokenType::BYTES: {
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

Variable execute_instruction(std::shared_ptr<Instruction> instr, Scope *scope) {
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
    case Operation::ATTR: {
        CHECK_PARAM_SIZE(2);
        auto var_name = params[0]->var->to_str();
        auto attr_name = params[1]->var->to_str();
        auto scope_with_variable = scope->scopeWithVariable(var_name);
        if (!scope_with_variable) {
            scope_with_variable = scope->parentScope.lock()->impl;
        }
        auto var = scope_with_variable->vars.get(var_name);
        if (!var->has_attr(attr_name)) {
            var->set_attr(attr_name, NONE);
        }
        return var->get_attr(attr_name);
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
    case Operation::IN_ROUND_BRACKETS: {
        if (params.size() == 1) {
            return params[0]->execute(scope);
        }
    }
    case Operation::CALL: {
        CHECK_PARAM_SIZE(2);
        InstructionParams call_args;
        call_args.push_back(params[1]);
        auto func = std::dynamic_pointer_cast<FunctionVariable>(params[0]->execute(scope));
        return func->call((const InstructionParams)(params[1]->params), scope);
    }
    case Operation::FSTRING: {
        CHECK_PARAM_SIZE(1);
        std::string str = FStringFormatter(params[0]->execute(scope)->to_str()).format(scope);
        return NEW_STRING(str);
    }
    }
    return std::make_shared<NoneVariable>();
}

Instruction Instruction::fromTokenList(const TokenList &tokens) {
    auto begin = tokens.begin();
    auto end   = tokens.end();
    return fromTokenRange(begin, end, TokenType::NONE);
}

Instruction Instruction::fromTokenRange(std::vector<Token>::const_iterator &current,
                                        std::vector<Token>::const_iterator &end,
                                        TokenType endToken,
                                        ParsingConext context) {
    Instruction result;

    while (current != end) {
        if (current->type == endToken) {
            ++current;
            break;
        }

        switch (current->type) {
        case TokenType::OPENING_ROUND_BRACKET: {
            ++current;
            auto in_round_brackets = std::make_shared<Instruction>(fromTokenRange(current, end, TokenType::CLOSING_ROUND_BRACKET,
                                                                   ParsingConext::IN_ROUND_BRACKETS));
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

    // handle unary minus or plus as the first token
    if ((result.params.size() >= 1) && (result.params[0]->token.type == TokenType::OPERATOR) && ((result.params[0]->token.value == "-") || (result.params[0]->token.value == "+"))) {
        result.params.insert(result.params.begin(), std::make_shared<Instruction>(Token(TokenType::NUMBER, "0")));
    }

    // handle unary minus or plus after comma
    for (size_t i = 1; i < result.params.size(); ++i) {
        if ((result.params[i-1]->token.type == TokenType::COMMA) && ((result.params[i]->token.value == "-") || (result.params[i]->token.value == "+"))) {
            result.params.insert(result.params.begin() + i, std::make_shared<Instruction>(Token(TokenType::NUMBER, "0")));
        }
    }

    // f-string
    for (size_t i = 0; i < result.params.size(); ++i) {
        if ((result.params[i]->op == Operation::TOKEN) && (result.params[i]->token.type == TokenType::FSTRING)) {
            result.params[i]->op = Operation::FSTRING;
            Variable param = NEW_STRING(result.params[i]->token.value);
            result.params[i]->params.push_back(std::make_shared<Instruction>(param));
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

    groupByOperator({{".", Operation::ATTR}});
    groupByOperator({{"**", Operation::POW}});
    groupByOperator({{"*", Operation::MUL},
                     {"/", Operation::DIV},
                     {"//", Operation::INT_DIV},
                     {"%", Operation::MOD}});
    groupByOperator({{"+", Operation::ADD},
                     {"-", Operation::SUB}});
    groupByOperator({{"=", (context == ParsingConext::IN_ROUND_BRACKETS) ? Operation::KWARG : Operation::ASSIGN}});

    if (result.op == Operation::NONE && result.params.size() == 1) {
        Instruction tmp = result; // TODO: it prevents InstructionTest from corrupting memory. Check what is the root cause
        result = *result.params[0].get();
    }

    if (result.op == Operation::NONE && result.params.size() == 2 && result.params[0]->op == Operation::VAR_NAME && result.params[1]->op == Operation::IN_ROUND_BRACKETS) {
        // TODO: a call to a function with more than 1 arg
        result.op = Operation::CALL;
    }

    for (size_t i = 0; i + 1 < result.params.size(); ++i) {
        if ((result.params[i]->op == Operation::ATTR) && (result.params[i+1]->op == Operation::IN_ROUND_BRACKETS)) {
            InstructionParams params_for_new_instr;
            params_for_new_instr.push_back(result.params[i]);
            params_for_new_instr.push_back(result.params[i+1]);

            auto new_instr = std::make_shared<Instruction>(Operation::CALL, params_for_new_instr);
            result.params[i] = new_instr;
            result.params.erase(result.params.begin() + i + 1);
        }
    }

    std::function<void(Instruction *instr)> recursivelyParseCommaListInsideBrackets = [&](Instruction *instr) {
        bool in_brackets = (instr->op == Operation::IN_ROUND_BRACKETS)
                        || (instr->op == Operation::IN_SQUARE_BRACKETS)
                        || (instr->op == Operation::IN_CURLY_BRACKETS);

        if (in_brackets && (instr->params.size() == 1) && (instr->params[0]->op == Operation::NONE)) {
            bool ok = true;
            for (size_t i = 0; i < instr->params[0]->params.size(); ++i) {
                bool is_comma = instr->params[0]->params[i]->op == Operation::TOKEN && instr->params[0]->params[i]->token.type == TokenType::COMMA; //value == ",";
                if ((i % 2) != is_comma) {
                    ok = false;
                }
            }
            if (ok) {
                for (size_t i = instr->params[0]->params.size() / 2; i > 0; --i) {
                    instr->params[0]->params.erase(instr->params[0]->params.begin() + 2*i-1);
                }
                instr->params = instr->params[0]->params;
            }
        }

        for (auto param: instr->params) {
            recursivelyParseCommaListInsideBrackets(param.get());
        }
    };

    recursivelyParseCommaListInsideBrackets(&result);

    return result;
}

std::string Instruction::debug_string(int indent_level) {
    std::string result = "";
    for (size_t i = 0; i < indent_level; ++i) {
        result += "  ";
    }
    result += opToString(op) + (var ? " " + var->to_str() : "") + "\n";
    for (auto param: params) {
        result += param->debug_string(indent_level + 1);
    }
    return result;
}

} // namespace MiniPython
