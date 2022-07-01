#include "Instruction.h"

#include "Test.h"

#define ASSERT_IS_VAR(param, varname) \
    MY_ASSERT(param->op == Operation::VAR_NAME); \
    MY_ASSERT(param->var != nullptr); \
    MY_ASSERT(param->var->get_type() == VariableType::STRING); \
    MY_ASSERT(std::dynamic_pointer_cast<StringVariable>(param->var)->get_value() == varname);

#define ASSERT_IS_VALUE(param, TYPE1, TYPE2, VALUE) \
    MY_ASSERT(param->op == Operation::RET_VALUE); \
    MY_ASSERT(param->var != nullptr); \
    CHECK_VAR(param->var, TYPE1, TYPE2, VALUE);

void test_instruction() {
    // a = 3
    TokenList simpleAssignmentTokens = {
        {TokenType::IDENTIFIER, "a"},
        {TokenType::OPERATOR, "="},
        {TokenType::NUMBER, "3"},
    };

    Instruction simpleAssignmentInstruction = Instruction::fromTokenList(simpleAssignmentTokens);

    MY_ASSERT(simpleAssignmentInstruction.op == Operation::SET);
    MY_ASSERT(simpleAssignmentInstruction.params.size() == 2);

    if (simpleAssignmentInstruction.params.size() == 2) {
        MY_ASSERT(simpleAssignmentInstruction.params[0]->op == Operation::VAR_NAME);
        ASSERT_IS_VAR(simpleAssignmentInstruction.params[0], "a");
        MY_ASSERT(simpleAssignmentInstruction.params[1]->op == Operation::RET_VALUE);
        ASSERT_IS_VALUE(simpleAssignmentInstruction.params[1], INT, Int, 3);
    }
}
