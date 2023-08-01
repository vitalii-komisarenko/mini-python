#include "Instruction.h"

#include "Test.h"

#define ASSERT_IS_VAR(param, varname) \
    MY_ASSERT_EQUAL(param->op, Operation::VAR_NAME); \
    if (param->op == Operation::VAR_NAME) { \
        MY_ASSERT(param->var != nullptr); \
        MY_ASSERT(param->var->get_type() == VariableType::STRING); \
        MY_ASSERT(std::dynamic_pointer_cast<StringVariable>(param->var)->get_value() == varname); \
    }

#define ASSERT_IS_VALUE(param, TYPE1, TYPE2, VALUE) \
    MY_ASSERT(param->op == Operation::RET_VALUE); \
    MY_ASSERT(param->var != nullptr); \
    if (param->var) { \
        CHECK_VAR(param->var, TYPE1, TYPE2, VALUE); \
    }

std::ostream& operator<< (std::ostream& os, Operation op) {
    os << "Operation::" << opToString(op);
    return os;
}

void test_instruction() {
    {
        // a = 3
        TokenList simpleAssignmentTokens = {
            {TokenType::IDENTIFIER, "a"},
            {TokenType::OPERATOR, "="},
            {TokenType::NUMBER, "3"},
        };

        Instruction simpleAssignmentInstruction = Instruction::fromTokenList(simpleAssignmentTokens);

        MY_ASSERT_EQUAL(simpleAssignmentInstruction.op, Operation::ASSIGN);
        MY_ASSERT_EQUAL(simpleAssignmentInstruction.params.size(), 2);

        if (simpleAssignmentInstruction.params.size() == 2) {
            MY_ASSERT_EQUAL(simpleAssignmentInstruction.params[0]->op, Operation::VAR_NAME);
            ASSERT_IS_VAR(simpleAssignmentInstruction.params[0], "a");
            MY_ASSERT_EQUAL(simpleAssignmentInstruction.params[1]->op, Operation::RET_VALUE);
            ASSERT_IS_VALUE(simpleAssignmentInstruction.params[1], INT, Int, 3);
        }
    }

    {
        // a = b + c
        TokenList tokens = {
            {TokenType::IDENTIFIER, "a"},
            {TokenType::OPERATOR, "="},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::OPERATOR, "+"},
            {TokenType::IDENTIFIER, "c"},
        };

        Instruction instr = Instruction::fromTokenList(tokens);

        MY_ASSERT_EQUAL(instr.op, Operation::ASSIGN);
        MY_ASSERT_EQUAL(instr.params.size(), 2);

        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "a");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::ADD);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 2);
            if (instr.params[1]->params.size() == 2) {
                ASSERT_IS_VAR(instr.params[1]->params[0], "b");
                ASSERT_IS_VAR(instr.params[1]->params[1], "c");
            }
        }
    }

    {
        // x = a * b + c
        TokenList tokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::OPERATOR, "="},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::OPERATOR, "*"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::OPERATOR, "+"},
            {TokenType::IDENTIFIER, "c"},
        };

        Instruction instr = Instruction::fromTokenList(tokens);

        MY_ASSERT_EQUAL(instr.op, Operation::ASSIGN);
        MY_ASSERT_EQUAL(instr.params.size(), 2);

        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "x");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::ADD);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 2);
            if (instr.params[1]->params.size() == 2) {
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->op, Operation::MUL);
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->params.size(), 2);
                if (instr.params[1]->params[0]->params.size() == 2) {
                    ASSERT_IS_VAR(instr.params[1]->params[0]->params[0], "a");
                    ASSERT_IS_VAR(instr.params[1]->params[0]->params[1], "b");
                }
                ASSERT_IS_VAR(instr.params[1]->params[1], "c");
            }
        }
    }

    {
        // x = a + b * c
        TokenList tokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::OPERATOR, "="},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::OPERATOR, "*"},
            {TokenType::IDENTIFIER, "c"},
        };

        Instruction instr = Instruction::fromTokenList(tokens);

        MY_ASSERT_EQUAL(instr.op, Operation::ASSIGN);
        MY_ASSERT_EQUAL(instr.params.size(), 2);
        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "x");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::ADD);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 2);
            if (instr.params[1]->params.size() == 2) {
                ASSERT_IS_VAR(instr.params[1]->params[0], "a");
                MY_ASSERT_EQUAL(instr.params[1]->params[1]->op, Operation::MUL);
                MY_ASSERT_EQUAL(instr.params[1]->params[1]->params.size(), 2);
                if (instr.params[1]->params[1]->params.size() == 2) {
                    ASSERT_IS_VAR(instr.params[1]->params[1]->params[0], "b");
                    ASSERT_IS_VAR(instr.params[1]->params[1]->params[1], "c");
                }
            }
        }
    }

    {
        // x = a * b + c
        TokenList tokens = {
            {TokenType::IDENTIFIER, "x"},
            {TokenType::OPERATOR, "="},
            {TokenType::IDENTIFIER, "a"},
            {TokenType::OPERATOR, "+"},
            {TokenType::IDENTIFIER, "b"},
            {TokenType::OPERATOR, "+"},
            {TokenType::IDENTIFIER, "c"},
        };

        Instruction instr = Instruction::fromTokenList(tokens);

        MY_ASSERT_EQUAL(instr.op, Operation::ASSIGN);
        MY_ASSERT_EQUAL(instr.params.size(), 2);

        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "x");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::ADD);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 2);
            if (instr.params[1]->params.size() == 2) {
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->op, Operation::ADD);
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->params.size(), 2);
                if (instr.params[1]->params[0]->params.size() == 2) {
                    ASSERT_IS_VAR(instr.params[1]->params[0]->params[0], "a");
                    ASSERT_IS_VAR(instr.params[1]->params[0]->params[1], "b");
                }
                ASSERT_IS_VAR(instr.params[1]->params[1], "c");
            }
        }
    }

    {
        // print("Hello, World!")
        TokenList tokens = {
            {TokenType::IDENTIFIER, "print"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::STRING, "Hello, World!"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
        };

        Instruction instr = Instruction::fromTokenList(tokens);

        MY_ASSERT_EQUAL(instr.op, Operation::CALL);
        MY_ASSERT_EQUAL(instr.params.size(), 2);

        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "print");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::RET_VALUE);
            ASSERT_IS_VALUE(instr.params[1], STRING, String, "Hello, World!");
        }
    }

    {
        // print(1 + 2)
        TokenList tokens = {
            {TokenType::IDENTIFIER, "print"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::NUMBER, "1"},
            {TokenType::OPERATOR, "+"},
            {TokenType::NUMBER, "2"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
        };

        Instruction instr = Instruction::fromTokenList(tokens);
        std::cout << instr.debug_string() << "\n";

        MY_ASSERT_EQUAL(instr.op, Operation::CALL);
        MY_ASSERT_EQUAL(instr.params.size(), 2);

        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "print");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::ADD);
        }
    }
}
