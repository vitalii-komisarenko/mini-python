#include "Instruction.h"

#include "Test.h"

#include <gtest/gtest.h>

#define ASSERT_IS_VAR(param, varname) \
    MY_ASSERT_EQUAL(param->op, Operation::VAR_NAME); \
    if (param->op == Operation::VAR_NAME) { \
        MY_ASSERT(param->var != nullptr); \
        MY_ASSERT(param->var->get_type() == VariableType::STRING); \
        MY_ASSERT(std::dynamic_pointer_cast<StringVariable>(param->var)->get_value() == varname); \
    }

static void EXPECT_IS_VAR(const Instruction &instr, const std::string &varname) {
    EXPECT_EQ(instr.op, Operation::VAR_NAME);
    EXPECT_NE(instr.var, nullptr);
    EXPECT_EQ(instr.var->get_type(), VariableType::STRING);
    EXPECT_EQ(std::dynamic_pointer_cast<StringVariable>(instr.var)->get_value(), varname);
}

static void EXPECT_IS_VAR(std::shared_ptr<Instruction> &instr, const std::string &varname) {
    EXPECT_IS_VAR(*(instr.get()), varname);
}

#define ASSERT_IS_VALUE(param, TYPE1, TYPE2, VALUE) \
    MY_ASSERT(param->op == Operation::RET_VALUE); \
    MY_ASSERT(param->var != nullptr); \
    if (param->var) { \
        CHECK_VAR(param->var, TYPE1, TYPE2, VALUE); \
    }

static void EXPECT_IS_VALUE(std::shared_ptr<Instruction> &instr, const Variable &var) {
    EXPECT_EQ(instr->op, Operation::RET_VALUE);
    EXPECT_NE(instr->var, nullptr);
    EXPECT_TRUE(instr->var->strictly_equal(var));
}

std::ostream& operator<< (std::ostream& os, Operation op) {
    os << "Operation::" << opToString(op);
    return os;
}

class InstructionTest: public testing::Test {
};

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
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 1);
            if (instr.params[0]->params.size() == 1) {
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->op, Operation::RET_VALUE);
                ASSERT_IS_VALUE(instr.params[1]->params[0], STRING, String, "Hello, World!");
            }
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

        MY_ASSERT_EQUAL(instr.op, Operation::CALL);
        MY_ASSERT_EQUAL(instr.params.size(), 2);

        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "print");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 1);
            if (instr.params[1]->params.size() == 1) {
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->op, Operation::ADD);
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->params.size(), 2);
                if (instr.params[1]->params[0]->params.size() == 2) {
                    MY_ASSERT_EQUAL(instr.params[1]->params[0]->params[0]->op, Operation::RET_VALUE);
                    ASSERT_IS_VALUE(instr.params[1]->params[0]->params[0], INT, Int, 1);
                    MY_ASSERT_EQUAL(instr.params[1]->params[0]->params[1]->op, Operation::RET_VALUE);
                    ASSERT_IS_VALUE(instr.params[1]->params[0]->params[1], INT, Int, 2);
                }
            }
        }
    }

    {
        // print("Hello,", "World!")
        TokenList tokens = {
            {TokenType::IDENTIFIER, "print"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::STRING, "Hello,"},
            {TokenType::COMMA, ""},
            {TokenType::STRING, "World!"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
        };

        Instruction instr = Instruction::fromTokenList(tokens);

        MY_ASSERT_EQUAL(instr.op, Operation::CALL);
        MY_ASSERT_EQUAL(instr.params.size(), 2);

        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "print");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 2);
            if (instr.params[1]->params.size() == 2) {
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->op, Operation::RET_VALUE);
                ASSERT_IS_VALUE(instr.params[1]->params[0], STRING, String, "Hello,");
                MY_ASSERT_EQUAL(instr.params[1]->params[1]->op, Operation::RET_VALUE);
                ASSERT_IS_VALUE(instr.params[1]->params[1], STRING, String, "World!");
            }
        }
    }

    {
        // print(-1)
        TokenList tokens = {
            {TokenType::IDENTIFIER, "print"},
            {TokenType::OPENING_ROUND_BRACKET, ""},
            {TokenType::OPERATOR, "-"},
            {TokenType::NUMBER, "1"},
            {TokenType::CLOSING_ROUND_BRACKET, ""},
        };

        Instruction instr = Instruction::fromTokenList(tokens);

        MY_ASSERT_EQUAL(instr.op, Operation::CALL);
        MY_ASSERT_EQUAL(instr.params.size(), 2);
        if (instr.params.size() == 2) {
            ASSERT_IS_VAR(instr.params[0], "print");
            MY_ASSERT_EQUAL(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
            MY_ASSERT_EQUAL(instr.params[1]->params.size(), 1);
            if (instr.params[1]->params.size() == 1) {
                MY_ASSERT_EQUAL(instr.params[1]->params[0]->op, Operation::RET_VALUE);
                ASSERT_IS_VALUE(instr.params[1]->params[0], INT, Int, -1);
            }
        }
    }
}

TEST_F(InstructionTest, attribute) {
    TokenList tokens = {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "."},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::OPERATOR, "="},
        {TokenType::NUMBER, "5"},
    };

    Instruction instr = Instruction::fromTokenList(tokens);

    EXPECT_EQ(instr.op, Operation::ASSIGN);
    EXPECT_EQ(instr.params.size(), 2);

    EXPECT_EQ(instr.params[0]->op, Operation::ATTR);
    EXPECT_EQ(instr.params[0]->params.size(), 2);

    EXPECT_IS_VAR(instr.params[0]->params[0], "x");
    EXPECT_IS_VAR(instr.params[0]->params[1], "y");

    EXPECT_IS_VALUE(instr.params[1], NEW_INT(5));
}
