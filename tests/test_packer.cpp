#include "Packer.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <utility>

class PackerTest : public ::testing::Test {
protected:
    Packer packer;

    // 在此处可以设置测试用的文件数据
    std::vector<std::pair<std::string, std::string>> testFiles = {
        {"file1.txt", "Hello World"},
        {"folder/file2.txt", "Test data"}
    };
};

// 测试打包功能
TEST_F(PackerTest, TestPack) {
    std::string packedData = packer.pack(testFiles);
    // 检查打包后的数据是否非空
    EXPECT_FALSE(packedData.empty());
}

// 测试解包功能
TEST_F(PackerTest, TestUnpack) {
    std::string packedData = packer.pack(testFiles);
    auto unpackedFiles = packer.unpack(packedData);

    // 检查解包后的数据数量是否与原始文件数据数量相同
    EXPECT_EQ(unpackedFiles.size(), testFiles.size());

    // 检查每个解包后的文件是否与原始文件相匹配
    for (size_t i = 0; i < testFiles.size(); ++i) {
        EXPECT_EQ(unpackedFiles[i].first, testFiles[i].first);
        EXPECT_EQ(unpackedFiles[i].second, testFiles[i].second);
    }
}

// 添加更多测试用例以检验不同数据的打包和解包结果
TEST_F(PackerTest, TestPack2) {
    std::string packedData = packer.pack(testFiles);

    // 检查打包后的数据是否非空
    EXPECT_FALSE(packedData.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
