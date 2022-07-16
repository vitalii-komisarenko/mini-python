#include "Scope.h"
#include "LineLevelParser.h"

#include "Test.h"

void test_scope() {
    auto echo = [](const InstructionParams &params) {
        return params[0]->execute();
    };
    auto echoVar = std::make_shared<FunctionVariable>(*echo);
    {
        Scope topLevelScope;

        topLevelScope.setVariable("echo", echoVar);

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

        intermediateScope.setVariable("foo", std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>("Bar")));
        MUST_THROW(topLevelScope.getVariable("foo"));
        CHECK_VAR(intermediateScope.getVariable("foo"), STRING, String, "Bar");
        CHECK_VAR(bottomLevelScope.getVariable("foo"), STRING, String, "Bar");

        MUST_THROW(topLevelScope.getVariable("this_var_not_set"));
        MUST_THROW(intermediateScope.getVariable("this_var_not_set"));
        MUST_THROW(bottomLevelScope.getVariable("this_var_not_set"));
    }

    // Parsing test
    {
        Lines lines = {
            "a = 5",
            "if a == 5:",
            "    print(5)"
            "    a = 6"
        };
        LineTree lineTree(lines);
        Scope scope(lineTree);

        MY_ASSERT_EQUAL(scope.impl->children.size(), 2);
        if (scope.impl->children.size() == 2) {
            MY_ASSERT_EQUAL(scope.impl->children[0].impl->children.size(), 0);
            MY_ASSERT_EQUAL(scope.impl->children[1].impl->children.size(), 2);
        }
    }
}
