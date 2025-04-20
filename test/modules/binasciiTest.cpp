#include "modules/Module.h"

#include <gtest/gtest.h>

using namespace MiniPython;

class HexlifyTest: public testing::Test {
};

TEST_F(HexlifyTest, simple_variation) {
    EXPECT_EQ(binascii::helper_hexlify("\xb9\x01\xef", "", 1), "b901ef");
}

TEST_F(HexlifyTest, separator_no_grouping) {
    EXPECT_EQ(binascii::helper_hexlify("\xb9\x01\xef", "-", 1), "b9-01-ef");
}

TEST_F(HexlifyTest, grouping_from_right) {
    EXPECT_EQ(binascii::helper_hexlify("abcdefghijklm", "_", 2), "61_6263_6465_6667_6869_6a6b_6c6d");
    EXPECT_EQ(binascii::helper_hexlify("abcdefghijklm", "_", 3), "61_626364_656667_68696a_6b6c6d");
    EXPECT_EQ(binascii::helper_hexlify("abcdefgh", "_", 4), "61626364_65666768");
}

TEST_F(HexlifyTest, grouping_from_left) {
    EXPECT_EQ(binascii::helper_hexlify("abcdefghijklm", "_", -2), "6162_6364_6566_6768_696a_6b6c_6d");
    EXPECT_EQ(binascii::helper_hexlify("abcdefghijklm", "_", -3), "616263_646566_676869_6a6b6c_6d");
    EXPECT_EQ(binascii::helper_hexlify("abcdefgh", "_", -4), "61626364_65666768");
}
