#include "Scope.h"
#include "LineLevelParser.h"

#include <gtest/gtest.h>

using namespace MiniPython;

#define CHECK_VAR(_VAR, TYPE1, TYPE2, VALUE)                                               \
    {                                                                                      \
        EXPECT_EQ(_VAR->get_type(), VariableType::TYPE1);                                  \
        auto converted = std::dynamic_pointer_cast<TYPE2 ## Variable>(_VAR);               \
        EXPECT_EQ(std::dynamic_pointer_cast<TYPE2 ## Variable>(_VAR)->get_value(), VALUE); \
    }

class ScopeTest: public testing::Test {
};

TEST_F(ScopeTest, test_1) {
    auto echo = [](const InstructionParams &params, Scope *scope) {
        return params[0]->execute(scope);
    };
    auto echoVar = std::make_shared<FunctionVariable>(*echo);

    auto topLevelScope = std::make_shared<Scope>();

    topLevelScope->setVariable("echo", echoVar);

    auto intermediateScope = std::make_shared<Scope>();
    topLevelScope->addChild(intermediateScope);

    auto bottomLevelScope = std::make_shared<Scope>();
    intermediateScope->addChild(bottomLevelScope);

    Variable x = std::static_pointer_cast<GenericVariable>(std::make_shared<IntVariable>(3));
    auto instr = std::make_shared<Instruction>();
    instr->op = Operation::RET_VALUE;
    instr->var = x;

    InstructionParams params;
    params.push_back(instr);

    CHECK_VAR(topLevelScope->call("echo", params), INT, Int, 3);
    CHECK_VAR(intermediateScope->call("echo", params), INT, Int, 3);
    CHECK_VAR(bottomLevelScope->call("echo", params), INT, Int, 3);

    intermediateScope->setVariable("foo", std::static_pointer_cast<GenericVariable>(std::make_shared<StringVariable>("Bar")));
    EXPECT_ANY_THROW(topLevelScope->getVariable("foo"));
    CHECK_VAR(intermediateScope->getVariable("foo"), STRING, String, "Bar");
    CHECK_VAR(bottomLevelScope->getVariable("foo"), STRING, String, "Bar");

    EXPECT_ANY_THROW(topLevelScope->getVariable("this_var_not_set"));
    EXPECT_ANY_THROW(intermediateScope->getVariable("this_var_not_set"));
    EXPECT_ANY_THROW(bottomLevelScope->getVariable("this_var_not_set"));
}

TEST_F(ScopeTest, parsing_test) {
    Lines lines = {
        "a = 5",
        "if a == 5:",
        "    print(5)",
        "    a = 6",
    };
    LineTree lineTree(lines);

    EXPECT_EQ(lineTree.children.size(), 2);
    EXPECT_EQ(lineTree.children[0]->children.size(), 0);
    EXPECT_EQ(lineTree.children[1]->children.size(), 2);

    auto scope = makeScope(lineTree);

    EXPECT_EQ(scope->impl->type, ScopeType::TOP_LEVEL);
    EXPECT_EQ(scope->impl->children.size(), 2);

    EXPECT_EQ(scope->impl->children[0]->impl->type, ScopeType::ORDINARY_LINE);
    EXPECT_EQ(scope->impl->children[0]->impl->children.size(), 0);
    EXPECT_EQ(scope->impl->children[0]->impl->instruction.op, Operation::ASSIGN);

    EXPECT_EQ(scope->impl->children[1]->impl->type, ScopeType::IF);
    EXPECT_EQ(scope->impl->children[1]->impl->children.size(), 2);
}

TEST_F(ScopeTest, function_call_one_arg) {
    Lines lines = {"print('Hello, World!')"};
    LineTree lineTree(lines);

    auto scope = makeScope(lineTree);

    EXPECT_EQ(scope->impl->type, ScopeType::TOP_LEVEL);
    EXPECT_EQ(scope->impl->children.size(), 1);

    EXPECT_EQ(scope->impl->children[0]->impl->type, ScopeType::ORDINARY_LINE);
    EXPECT_EQ(scope->impl->children[0]->impl->instruction.op, Operation::CALL);
}
