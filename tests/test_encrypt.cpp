#include <gtest/gtest.h>
#include "Encryptor.h"  // 替换为您的 Encryptor 类的实际路径

class EncryptorTest : public ::testing::Test {
protected:
    Encryptor encryptor;  // Encryptor 实例

    // 可以在此处设置测试环境
    void SetUp() override {
        // 为测试设置一个固定的密钥
        encryptor.setKey("testkey");
    }
};

TEST_F(EncryptorTest, test_encrypt) {
    std::string originalText = "Hello, World!";
    std::string encryptedText = encryptor.encrypt(originalText);
    std::string decryptedText = encryptor.decrypt(encryptedText);

    // 验证解密后的文本是否与原始文本相同
    ASSERT_EQ(decryptedText, originalText);
}
