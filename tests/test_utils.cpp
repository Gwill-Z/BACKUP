#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "utils.h"

class FileFunctionsTest : public ::testing::Test {
protected:
    std::string testFilePath = "test_file.txt";
    std::string testData = "Hello, World!";

    void SetUp() override {
        // 创建并写入测试文件
        std::ofstream outFile(testFilePath);
        outFile << testData;
        outFile.close();
    }

    void TearDown() override {
        // 删除测试文件
        std::filesystem::remove(testFilePath);
    }
};

// 测试获取文件大小
TEST_F(FileFunctionsTest, GetFileSize) {
    long size = getFileSize(testFilePath);
    EXPECT_EQ(size, static_cast<long>(testData.size()));
}

// 测试获取文件最后修改时间
// 注意：这个测试可能会受到文件系统精度的影响
TEST_F(FileFunctionsTest, GetFileLastModifiedTime) {
    std::string lastModifiedTime = getFileLastModifiedTime(testFilePath);
    EXPECT_FALSE(lastModifiedTime.empty());
}

