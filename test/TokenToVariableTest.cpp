#include "TokenToVariable.h"

#include <gtest/gtest.h>

using namespace MiniPython;

#define TOKEN(TYPE, VALUE) \
    parseTokenToVariable({TokenType::TYPE, VALUE})

class TokenToVariableTest: public testing::Test {
};

TEST_F(TokenToVariableTest, token_to_variable) {
    EXPECT_TRUE(TOKEN(STRING, "str")->strictly_equal(NEW_STRING("str")));
    EXPECT_TRUE(TOKEN(NUMBER, "0")->strictly_equal(NEW_INT(0)));
    EXPECT_TRUE(TOKEN(NUMBER, "0.0")->strictly_equal(NEW_FLOAT(0)));
    EXPECT_TRUE(TOKEN(NUMBER, ".0")->strictly_equal(NEW_FLOAT(0)));
    EXPECT_TRUE(TOKEN(NUMBER, "0.")->strictly_equal(NEW_FLOAT(0)));
    EXPECT_TRUE(TOKEN(NUMBER, "0.e0")->strictly_equal(NEW_FLOAT(0)));
    EXPECT_TRUE(TOKEN(NUMBER, "0.e-0")->strictly_equal(NEW_FLOAT(0)));
    EXPECT_TRUE(TOKEN(NUMBER, "0.E+0")->strictly_equal(NEW_FLOAT(0)));
    EXPECT_TRUE(TOKEN(NUMBER, "1e4")->strictly_equal(NEW_FLOAT(10000)));
    EXPECT_TRUE(TOKEN(NUMBER, "123456e-01")->strictly_equal(NEW_FLOAT(12345.6)));
    EXPECT_TRUE(TOKEN(NUMBER, "1000000000000000")->strictly_equal(NEW_INT(1000000000000000)));
    EXPECT_TRUE(TOKEN(NUMBER, "0o123456")->strictly_equal(NEW_INT(42798)));
    EXPECT_TRUE(TOKEN(NUMBER, "0xFFFFFFFF")->strictly_equal(NEW_INT(4294967295)));
}
