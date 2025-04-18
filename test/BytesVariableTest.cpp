#include "variable/Variable.h"

#include <gtest/gtest.h>

using namespace MiniPython;

class BytesStringificationTest: public testing::Test {
};

TEST_F(BytesStringificationTest, stringification_quotes) {
    EXPECT_EQ(NEW_BYTES("'")->to_str(), "b\"'\"");
    EXPECT_EQ(NEW_BYTES("\"")->to_str(), "b'\"'");
    EXPECT_EQ(NEW_BYTES("\"'")->to_str(), "b'\"\\''");
}

TEST_F(BytesStringificationTest, stringification_ascii) {
    EXPECT_EQ(NEW_BYTES(std::string(1, '\0'))->to_str(), "b'\\x00'");
    EXPECT_EQ(NEW_BYTES("\x01")->to_str(), "b'\\x01'");
    EXPECT_EQ(NEW_BYTES("\x02")->to_str(), "b'\\x02'");
    EXPECT_EQ(NEW_BYTES("\x03")->to_str(), "b'\\x03'");
    EXPECT_EQ(NEW_BYTES("\x04")->to_str(), "b'\\x04'");
    EXPECT_EQ(NEW_BYTES("\x05")->to_str(), "b'\\x05'");
    EXPECT_EQ(NEW_BYTES("\x06")->to_str(), "b'\\x06'");
    EXPECT_EQ(NEW_BYTES("\x07")->to_str(), "b'\\x07'");
    EXPECT_EQ(NEW_BYTES("\x08")->to_str(), "b'\\x08'");
    EXPECT_EQ(NEW_BYTES("\x09")->to_str(), "b'\\t'");
    EXPECT_EQ(NEW_BYTES("\x0a")->to_str(), "b'\\n'");
    EXPECT_EQ(NEW_BYTES("\x0b")->to_str(), "b'\\x0b'");
    EXPECT_EQ(NEW_BYTES("\x0c")->to_str(), "b'\\x0c'");
    EXPECT_EQ(NEW_BYTES("\x0d")->to_str(), "b'\\r'");
    EXPECT_EQ(NEW_BYTES("\x0e")->to_str(), "b'\\x0e'");
    EXPECT_EQ(NEW_BYTES("\x0f")->to_str(), "b'\\x0f'");
    EXPECT_EQ(NEW_BYTES("\x10")->to_str(), "b'\\x10'");
    EXPECT_EQ(NEW_BYTES("\x11")->to_str(), "b'\\x11'");
    EXPECT_EQ(NEW_BYTES("\x12")->to_str(), "b'\\x12'");
    EXPECT_EQ(NEW_BYTES("\x13")->to_str(), "b'\\x13'");
    EXPECT_EQ(NEW_BYTES("\x14")->to_str(), "b'\\x14'");
    EXPECT_EQ(NEW_BYTES("\x15")->to_str(), "b'\\x15'");
    EXPECT_EQ(NEW_BYTES("\x16")->to_str(), "b'\\x16'");
    EXPECT_EQ(NEW_BYTES("\x17")->to_str(), "b'\\x17'");
    EXPECT_EQ(NEW_BYTES("\x18")->to_str(), "b'\\x18'");
    EXPECT_EQ(NEW_BYTES("\x19")->to_str(), "b'\\x19'");
    EXPECT_EQ(NEW_BYTES("\x1a")->to_str(), "b'\\x1a'");
    EXPECT_EQ(NEW_BYTES("\x1b")->to_str(), "b'\\x1b'");
    EXPECT_EQ(NEW_BYTES("\x1c")->to_str(), "b'\\x1c'");
    EXPECT_EQ(NEW_BYTES("\x1d")->to_str(), "b'\\x1d'");
    EXPECT_EQ(NEW_BYTES("\x1e")->to_str(), "b'\\x1e'");
    EXPECT_EQ(NEW_BYTES("\x1f")->to_str(), "b'\\x1f'");
    EXPECT_EQ(NEW_BYTES("\x20")->to_str(), "b' '");
    EXPECT_EQ(NEW_BYTES("\x21")->to_str(), "b'!'");
    // Skipping "\x22": quotes are covered in other test
    EXPECT_EQ(NEW_BYTES("\x23")->to_str(), "b'#'");
    EXPECT_EQ(NEW_BYTES("\x24")->to_str(), "b'$'");
    EXPECT_EQ(NEW_BYTES("\x25")->to_str(), "b'%'");
    EXPECT_EQ(NEW_BYTES("\x26")->to_str(), "b'&'");
    // Skipping "\x27": quotes are covered in other test
    EXPECT_EQ(NEW_BYTES("\x28")->to_str(), "b'('");
    EXPECT_EQ(NEW_BYTES("\x29")->to_str(), "b')'");
    EXPECT_EQ(NEW_BYTES("\x2a")->to_str(), "b'*'");
    EXPECT_EQ(NEW_BYTES("\x2b")->to_str(), "b'+'");
    EXPECT_EQ(NEW_BYTES("\x2c")->to_str(), "b','");
    EXPECT_EQ(NEW_BYTES("\x2d")->to_str(), "b'-'");
    EXPECT_EQ(NEW_BYTES("\x2e")->to_str(), "b'.'");
    EXPECT_EQ(NEW_BYTES("\x2f")->to_str(), "b'/'");
    EXPECT_EQ(NEW_BYTES("\x30")->to_str(), "b'0'"); 
    EXPECT_EQ(NEW_BYTES("\x31")->to_str(), "b'1'");
    EXPECT_EQ(NEW_BYTES("\x32")->to_str(), "b'2'");
    EXPECT_EQ(NEW_BYTES("\x33")->to_str(), "b'3'");
    EXPECT_EQ(NEW_BYTES("\x34")->to_str(), "b'4'");
    EXPECT_EQ(NEW_BYTES("\x35")->to_str(), "b'5'");
    EXPECT_EQ(NEW_BYTES("\x36")->to_str(), "b'6'");
    EXPECT_EQ(NEW_BYTES("\x37")->to_str(), "b'7'");
    EXPECT_EQ(NEW_BYTES("\x38")->to_str(), "b'8'");
    EXPECT_EQ(NEW_BYTES("\x39")->to_str(), "b'9'");
    EXPECT_EQ(NEW_BYTES("\x3a")->to_str(), "b':'");
    EXPECT_EQ(NEW_BYTES("\x3b")->to_str(), "b';'");
    EXPECT_EQ(NEW_BYTES("\x3c")->to_str(), "b'<'");
    EXPECT_EQ(NEW_BYTES("\x3d")->to_str(), "b'='");
    EXPECT_EQ(NEW_BYTES("\x3e")->to_str(), "b'>'");
    EXPECT_EQ(NEW_BYTES("\x3f")->to_str(), "b'?'");
    EXPECT_EQ(NEW_BYTES("\x40")->to_str(), "b'@'");
    EXPECT_EQ(NEW_BYTES("\x41")->to_str(), "b'A'");
    EXPECT_EQ(NEW_BYTES("\x42")->to_str(), "b'B'");
    EXPECT_EQ(NEW_BYTES("\x43")->to_str(), "b'C'");
    EXPECT_EQ(NEW_BYTES("\x44")->to_str(), "b'D'");
    EXPECT_EQ(NEW_BYTES("\x45")->to_str(), "b'E'");
    EXPECT_EQ(NEW_BYTES("\x46")->to_str(), "b'F'");
    EXPECT_EQ(NEW_BYTES("\x47")->to_str(), "b'G'");
    EXPECT_EQ(NEW_BYTES("\x48")->to_str(), "b'H'");
    EXPECT_EQ(NEW_BYTES("\x49")->to_str(), "b'I'");
    EXPECT_EQ(NEW_BYTES("\x4a")->to_str(), "b'J'");
    EXPECT_EQ(NEW_BYTES("\x4b")->to_str(), "b'K'");
    EXPECT_EQ(NEW_BYTES("\x4c")->to_str(), "b'L'");
    EXPECT_EQ(NEW_BYTES("\x4d")->to_str(), "b'M'");
    EXPECT_EQ(NEW_BYTES("\x4e")->to_str(), "b'N'");
    EXPECT_EQ(NEW_BYTES("\x4f")->to_str(), "b'O'");
    EXPECT_EQ(NEW_BYTES("\x50")->to_str(), "b'P'");
    EXPECT_EQ(NEW_BYTES("\x51")->to_str(), "b'Q'");
    EXPECT_EQ(NEW_BYTES("\x52")->to_str(), "b'R'");
    EXPECT_EQ(NEW_BYTES("\x53")->to_str(), "b'S'");
    EXPECT_EQ(NEW_BYTES("\x54")->to_str(), "b'T'");
    EXPECT_EQ(NEW_BYTES("\x55")->to_str(), "b'U'");
    EXPECT_EQ(NEW_BYTES("\x56")->to_str(), "b'V'");
    EXPECT_EQ(NEW_BYTES("\x57")->to_str(), "b'W'");
    EXPECT_EQ(NEW_BYTES("\x58")->to_str(), "b'X'");
    EXPECT_EQ(NEW_BYTES("\x59")->to_str(), "b'Y'");
    EXPECT_EQ(NEW_BYTES("\x5a")->to_str(), "b'Z'");
    EXPECT_EQ(NEW_BYTES("\x5b")->to_str(), "b'['");
    EXPECT_EQ(NEW_BYTES("\x5c")->to_str(), "b'\\'");
    EXPECT_EQ(NEW_BYTES("\x5d")->to_str(), "b']'");
    EXPECT_EQ(NEW_BYTES("\x5e")->to_str(), "b'^'");
    EXPECT_EQ(NEW_BYTES("\x5f")->to_str(), "b'_'");
    EXPECT_EQ(NEW_BYTES("\x60")->to_str(), "b'`'");
    EXPECT_EQ(NEW_BYTES("\x61")->to_str(), "b'a'");
    EXPECT_EQ(NEW_BYTES("\x62")->to_str(), "b'b'");
    EXPECT_EQ(NEW_BYTES("\x63")->to_str(), "b'c'");
    EXPECT_EQ(NEW_BYTES("\x64")->to_str(), "b'd'");
    EXPECT_EQ(NEW_BYTES("\x65")->to_str(), "b'e'");
    EXPECT_EQ(NEW_BYTES("\x66")->to_str(), "b'f'");
    EXPECT_EQ(NEW_BYTES("\x67")->to_str(), "b'g'");
    EXPECT_EQ(NEW_BYTES("\x68")->to_str(), "b'h'");
    EXPECT_EQ(NEW_BYTES("\x69")->to_str(), "b'i'");
    EXPECT_EQ(NEW_BYTES("\x6a")->to_str(), "b'j'");
    EXPECT_EQ(NEW_BYTES("\x6b")->to_str(), "b'k'");
    EXPECT_EQ(NEW_BYTES("\x6c")->to_str(), "b'l'");
    EXPECT_EQ(NEW_BYTES("\x6d")->to_str(), "b'm'");
    EXPECT_EQ(NEW_BYTES("\x6e")->to_str(), "b'n'");
    EXPECT_EQ(NEW_BYTES("\x6f")->to_str(), "b'o'");
    EXPECT_EQ(NEW_BYTES("\x70")->to_str(), "b'p'");
    EXPECT_EQ(NEW_BYTES("\x71")->to_str(), "b'q'");
    EXPECT_EQ(NEW_BYTES("\x72")->to_str(), "b'r'");
    EXPECT_EQ(NEW_BYTES("\x73")->to_str(), "b's'");
    EXPECT_EQ(NEW_BYTES("\x74")->to_str(), "b't'");
    EXPECT_EQ(NEW_BYTES("\x75")->to_str(), "b'u'");
    EXPECT_EQ(NEW_BYTES("\x76")->to_str(), "b'v'");
    EXPECT_EQ(NEW_BYTES("\x77")->to_str(), "b'w'");
    EXPECT_EQ(NEW_BYTES("\x78")->to_str(), "b'x'");
    EXPECT_EQ(NEW_BYTES("\x79")->to_str(), "b'y'");
    EXPECT_EQ(NEW_BYTES("\x7a")->to_str(), "b'z'");
    EXPECT_EQ(NEW_BYTES("\x7b")->to_str(), "b'{'");
    EXPECT_EQ(NEW_BYTES("\x7c")->to_str(), "b'|'");
    EXPECT_EQ(NEW_BYTES("\x7d")->to_str(), "b'}'");
    EXPECT_EQ(NEW_BYTES("\x7e")->to_str(), "b'~'");
    EXPECT_EQ(NEW_BYTES("\x7f")->to_str(), "b'\\x7f'");
}

TEST_F(BytesStringificationTest, stringification_non_ascii) {
    EXPECT_EQ(NEW_BYTES("\x80")->to_str(), "b'\\x80'");
    EXPECT_EQ(NEW_BYTES("\xac")->to_str(), "b'\\xac'");
    EXPECT_EQ(NEW_BYTES("\xff")->to_str(), "b'\\xff'");
}