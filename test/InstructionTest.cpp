#include "Instruction.h"

#include "Test.h"

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
        MY_ASSERT(simpleAssignmentInstruction.params[1]->op == Operation::RET_VALUE);
    }
}
