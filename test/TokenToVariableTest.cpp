#include "TokenToVariable.h"

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
    CHECK_VAR(TOKEN(NUMBER, "123456e-01"), FLOAT, Float, 12345.6);
    CHECK_VAR(TOKEN(NUMBER, "1000000000000000"), INT, Int, 1000000000000000);
    CHECK_VAR(TOKEN(NUMBER, "0o123456"), INT, Int, 42798);
    CHECK_VAR(TOKEN(NUMBER, "0xFFFFFFFF"), INT, Int, 4294967295);
}
