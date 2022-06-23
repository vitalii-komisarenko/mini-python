#include "token2variable/TokenToVariable.h"

#include "Test.h"

#define TOKEN(TYPE, VALUE) \
    parseTokenToVariable({TokenType::TYPE, VALUE})

void test_token2variable() {
    CHECK_VAR(TOKEN(STRING, "str"), STRING, String, "str");
    CHECK_VAR(TOKEN(NUMBER, "0"), INT, Int, 0);
    CHECK_VAR(TOKEN(NUMBER, "0.0"), FLOAT, Float, 0);
    CHECK_VAR(TOKEN(NUMBER, ".0"), FLOAT, Float, 0);
    CHECK_VAR(TOKEN(NUMBER, "0."), FLOAT, Float, 0);
    CHECK_VAR(TOKEN(NUMBER, "0.e0"), FLOAT, Float, 0);
    CHECK_VAR(TOKEN(NUMBER, "0.e-0"), FLOAT, Float, 0);
    CHECK_VAR(TOKEN(NUMBER, "0.E+0"), FLOAT, Float, 0);
    CHECK_VAR(TOKEN(NUMBER, "1e4"), FLOAT, Float, 10000);
}
