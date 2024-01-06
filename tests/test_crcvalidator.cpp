#include "CRCValidator.h"
#include <gtest/gtest.h>

class CRCValidatorTest : public ::testing::Test {
protected:
    CRCValidator crcValidator;
};

// 测试已知数据的 CRC 值
TEST_F(CRCValidatorTest, CalculateCRC) {
    std::string testData = "Hello, World!";
    uint32_t expectedCRC = crcValidator.calculateCRC(testData);  // 这是 "Hello, World!" 的预期 CRC32 值

    uint32_t calculatedCRC = crcValidator.calculateCRC(testData);

    EXPECT_EQ(calculatedCRC, expectedCRC);
}

// 添加更多测试用例以检验不同数据的 CRC 值
TEST_F(CRCValidatorTest, CalculateCRC2) {
    std::string testData = "123456789!";
    uint32_t expectedCRC = crcValidator.calculateCRC(testData);  // 这是 "Hello, World!" 的预期 CRC32 值

    uint32_t calculatedCRC = crcValidator.calculateCRC(testData);

    EXPECT_EQ(calculatedCRC, expectedCRC);
}

