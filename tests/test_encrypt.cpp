#include <gtest/gtest.h>
#include "Encryptor.h"  // 替换为您的 Encryptor 类的实际路径

const std::string ENCRYPTION_MARKER = "[*ENCRYPTED-DATA*]";

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
    size_t keyEndPos = encryptedText.find('\0', ENCRYPTION_MARKER.size());
    std::string encryptedData = encryptedText.substr(keyEndPos + 1);
    std::string decryptedText = encryptor.decrypt(encryptedData);

    // 验证解密后的文本是否与原始文本相同
    ASSERT_EQ(decryptedText, originalText);
}

TEST_F(EncryptorTest, test_decrypt) {
    std::string originalText = "123456789!";
    std::string encryptedText = encryptor.encrypt(originalText);
    size_t keyEndPos = encryptedText.find('\0', ENCRYPTION_MARKER.size());
    std::string encryptedData = encryptedText.substr(keyEndPos + 1);
    std::string decryptedText = encryptor.decrypt(encryptedData);

    // 验证解密后的文本是否与原始文本相同
    ASSERT_EQ(decryptedText, originalText);
}
