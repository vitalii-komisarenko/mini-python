#include "Instruction.h"

#include "Test.h"

#include <gtest/gtest.h>

using namespace MiniPython;

/* Helper functions */

static void EXPECT_IS_BINARY_OP(const Instruction &instr, Operation op) {
    EXPECT_EQ(instr.op, op);
    EXPECT_EQ(instr.params.size(), 2);
}

static void EXPECT_IS_BINARY_OP(std::shared_ptr<Instruction> &instr, Operation op) {
    EXPECT_IS_BINARY_OP(*(instr.get()), op);
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

static void EXPECT_IS_VALUE(std::shared_ptr<Instruction> &instr, const Variable &var) {
    EXPECT_EQ(instr->op, Operation::RET_VALUE);
    EXPECT_NE(instr->var, nullptr);
    EXPECT_TRUE(instr->var->strictly_equal(var));
}

std::ostream& operator<< (std::ostream& os, Operation op) {
    os << "Operation::" << opToString(op);
    return os;
}

/* Tests */

class InstructionTest: public testing::Test {
};

TEST_F(InstructionTest, assignment) {
    // a = 3
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, "="},
        {TokenType::NUMBER, "3"},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::ASSIGN);
    EXPECT_IS_VAR(instr.params[0], "a");
    EXPECT_IS_VALUE(instr.params[1], NEW_INT(3));
}

TEST_F(InstructionTest, sum) {
    // a = b + c
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, "="},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::OPERATOR, "+"},
        {TokenType::IDENTIFIER, "c"},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::ASSIGN);
    EXPECT_IS_VAR(instr.params[0], "a");
    EXPECT_IS_BINARY_OP(instr.params[1], Operation::ADD);
    EXPECT_IS_VAR(instr.params[1]->params[0], "b");
    EXPECT_IS_VAR(instr.params[1]->params[1], "c");
}

TEST_F(InstructionTest, multiple_arithmetic_ops) {
    // x = a * b + c
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "="},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, "*"},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::OPERATOR, "+"},
        {TokenType::IDENTIFIER, "c"},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::ASSIGN);
    EXPECT_IS_VAR(instr.params[0], "x");
    EXPECT_IS_BINARY_OP(instr.params[1], Operation::ADD);
    EXPECT_IS_BINARY_OP(instr.params[1]->params[0], Operation::MUL);
    EXPECT_IS_VAR(instr.params[1]->params[0]->params[0], "a");
    EXPECT_IS_VAR(instr.params[1]->params[0]->params[1], "b");
    EXPECT_IS_VAR(instr.params[1]->params[1], "c");
}

TEST_F(InstructionTest, multiple_arithmetic_ops_2) {
    // x = a + b * c
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "="},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, "+"},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::OPERATOR, "*"},
        {TokenType::IDENTIFIER, "c"},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::ASSIGN);
    EXPECT_IS_VAR(instr.params[0], "x");
    EXPECT_IS_BINARY_OP(instr.params[1], Operation::ADD);
    EXPECT_IS_VAR(instr.params[1]->params[0], "a");
    EXPECT_IS_BINARY_OP(instr.params[1]->params[1], Operation::MUL);
    EXPECT_IS_VAR(instr.params[1]->params[1]->params[0], "b");
    EXPECT_IS_VAR(instr.params[1]->params[1]->params[1], "c");
}

TEST_F(InstructionTest, multiple_arithmetic_ops_3) {
    // x = a * b + c
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "="},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, "+"},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::OPERATOR, "+"},
        {TokenType::IDENTIFIER, "c"},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::ASSIGN);
    EXPECT_IS_VAR(instr.params[0], "x");
    EXPECT_IS_BINARY_OP(instr.params[1], Operation::ADD);
    EXPECT_IS_BINARY_OP(instr.params[1]->params[0], Operation::ADD);
    EXPECT_IS_VAR(instr.params[1]->params[0]->params[0], "a");
    EXPECT_IS_VAR(instr.params[1]->params[0]->params[1], "b");
    EXPECT_IS_VAR(instr.params[1]->params[1], "c");
}

TEST_F(InstructionTest, hello_world) {
    // print("Hello, World!")
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "print"},
        {TokenType::OPENING_ROUND_BRACKET, ""},
        {TokenType::STRING, "Hello, World!"},
        {TokenType::CLOSING_ROUND_BRACKET, ""},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::CALL);
    EXPECT_IS_VAR(instr.params[0], "print");
    EXPECT_EQ(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
    EXPECT_EQ(instr.params[1]->params.size(), 1);
    EXPECT_IS_VALUE(instr.params[1]->params[0], NEW_STRING("Hello, World!"));
}

TEST_F(InstructionTest, print_sum) {
    // print(1 + 2)
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "print"},
        {TokenType::OPENING_ROUND_BRACKET, ""},
        {TokenType::NUMBER, "1"},
        {TokenType::OPERATOR, "+"},
        {TokenType::NUMBER, "2"},
        {TokenType::CLOSING_ROUND_BRACKET, ""},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::CALL);
    EXPECT_IS_VAR(instr.params[0], "print");
    EXPECT_EQ(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
    EXPECT_EQ(instr.params[1]->params.size(), 1);
    EXPECT_IS_BINARY_OP(instr.params[1]->params[0], Operation::ADD);
    EXPECT_IS_VALUE(instr.params[1]->params[0]->params[0], NEW_INT(1));
    EXPECT_IS_VALUE(instr.params[1]->params[0]->params[1], NEW_INT(2));
}

TEST_F(InstructionTest, print_two_params) {
    // print("Hello,", "World!")
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "print"},
        {TokenType::OPENING_ROUND_BRACKET, ""},
        {TokenType::STRING, "Hello,"},
        {TokenType::COMMA, ""},
        {TokenType::STRING, "World!"},
        {TokenType::CLOSING_ROUND_BRACKET, ""},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::CALL);
    EXPECT_IS_VAR(instr.params[0], "print");
    EXPECT_EQ(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
    EXPECT_EQ(instr.params[1]->params.size(), 2);
    EXPECT_IS_VALUE(instr.params[1]->params[0], NEW_STRING("Hello,"));
    EXPECT_IS_VALUE(instr.params[1]->params[1], NEW_STRING("World!"));
}

TEST_F(InstructionTest, print_negative_number) {
    // print(-1)
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "print"},
        {TokenType::OPENING_ROUND_BRACKET, ""},
        {TokenType::OPERATOR, "-"},
        {TokenType::NUMBER, "1"},
        {TokenType::CLOSING_ROUND_BRACKET, ""},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::CALL);
    EXPECT_IS_VAR(instr.params[0], "print");
    EXPECT_EQ(instr.params[1]->op, Operation::IN_ROUND_BRACKETS);
    EXPECT_EQ(instr.params[1]->params.size(), 1);
    EXPECT_IS_BINARY_OP(instr.params[1]->params[0], Operation::SUB);
    EXPECT_IS_VALUE(instr.params[1]->params[0]->params[0], NEW_INT(0));
    EXPECT_IS_VALUE(instr.params[1]->params[0]->params[1], NEW_INT(1));
}

TEST_F(InstructionTest, attribute) {
    // x.y = 5
    Instruction instr = Instruction::fromTokenList({
        {TokenType::IDENTIFIER, "x"},
        {TokenType::OPERATOR, "."},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::OPERATOR, "="},
        {TokenType::NUMBER, "5"},
    });

    EXPECT_IS_BINARY_OP(instr, Operation::ASSIGN);
    EXPECT_IS_BINARY_OP(instr.params[0], Operation::ATTR);
    EXPECT_IS_VAR(instr.params[0]->params[0], "x");
    EXPECT_IS_VAR(instr.params[0]->params[1], "y");
    EXPECT_IS_VALUE(instr.params[1], NEW_INT(5));
}
