#include "FileManager.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

class FileManagerTest : public ::testing::Test {
protected:
    FileManager fileManager;
    std::string testFilePath;
    std::string testDirectoryPath;
    std::string testData;

    void SetUp() override {
        // 设置用于测试的文件和目录路径
        testFilePath = "test_file.txt";
        testDirectoryPath = "test_directory";
        testData = "Hello, World!";

        // 创建测试文件
        std::ofstream outFile(testFilePath);
        outFile << testData;
        outFile.close();
    }

    void TearDown() override {
        // 清理测试文件和目录
        std::filesystem::remove(testFilePath);
        std::filesystem::remove_all(testDirectoryPath);
    }
};

// 测试读取文件
TEST_F(FileManagerTest, ReadFile) {
    std::string content = fileManager.readFile(testFilePath);
    EXPECT_EQ(content, testData);
}

// 测试判断目录
TEST_F(FileManagerTest, IsDirectory) {
    EXPECT_FALSE(fileManager.isDirectory(testFilePath));
    std::filesystem::create_directory(testDirectoryPath);
    EXPECT_TRUE(fileManager.isDirectory(testDirectoryPath));
}

// 测试创建目录
TEST_F(FileManagerTest, CreateDirectory) {
    fileManager.createDirectory(testDirectoryPath);
    EXPECT_TRUE(std::filesystem::exists(testDirectoryPath));
    EXPECT_TRUE(std::filesystem::is_directory(testDirectoryPath));
}

// 测试恢复文件
TEST_F(FileManagerTest, RestoreFiles) {
    std::vector<std::pair<std::string, std::string>> files = {{testFilePath, testData}};
    fileManager.restoreFiles(files, testDirectoryPath);

    std::string restoredFilePath = testDirectoryPath + "/" + testFilePath;
    std::ifstream inFile(restoredFilePath);
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    EXPECT_EQ(buffer.str(), testData);
}
