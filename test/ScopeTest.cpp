#include "../scope/Scope.h"

#include "Test.h"

void test_scope() {
    auto echo = +[](const InstructionParams &params) {
        return params[0]->execute();
    };
    {
        Scope topLevelScope;

        topLevelScope.addBuiltInFunction("echo",  echo);

        Scope intermediateScope;
        topLevelScope.addChild(intermediateScope);

        Scope bottomLevelScope;
        intermediateScope.addChild(bottomLevelScope);

        Variable x = std::static_pointer_cast<GenericVariable>(std::make_shared<IntVariable>(3));
        auto instr = std::make_shared<Instruction>();
        instr->op = Operation::RET_VALUE;
        instr->var = x;

        InstructionParams params;
        params.push_back(instr);

        CHECK_VAR(topLevelScope.call("echo", params), INT, Int, 3);
        CHECK_VAR(intermediateScope.call("echo", params), INT, Int, 3);
        CHECK_VAR(bottomLevelScope.call("echo", params), INT, Int, 3);
    }
}
