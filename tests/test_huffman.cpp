#include <gtest/gtest.h>
#include "HuffmanNode.h"
#include "Compressor.h"

class HuffmanTest : public ::testing::Test {
protected:
    // 在此处设置测试环境，如果需要的话
};

TEST_F(HuffmanTest, SerializeDeserializeTest) {
    std::string test_str = "!/home/victor/BACKUP/data/test.txttest";

    std::unordered_map<char, int> frequencies;
    for (char ch : test_str) {
        frequencies[ch]++;
    }
    // 创建哈夫曼树
    Compressor compressor;
    compressor.buildTree(frequencies);


    // 序列化哈夫曼树
    std::string serialized = compressor.serializeTree(compressor.root);

    // 反序列化哈夫曼树
    size_t pos = 0;
    auto deserializedRoot = compressor.deserializeTree(serialized, pos);

    // 再次序列化用于验证
    std::string serializedAgain = compressor.serializeTree(deserializedRoot);

    // 检查两次序列化的结果是否相同
    ASSERT_EQ(serialized, serializedAgain);
}

TEST_F(HuffmanTest, CompressDecompressTest) {
    std::string test_str = "123zjltbjhsk111112354325";

    std::unordered_map<char, int> frequencies;
    for (char ch : test_str) {
        frequencies[ch]++;
    }
    // 创建哈夫曼树
    Compressor compressor;
    compressor.buildTree(frequencies);

    // 压缩数据
    std::string compressed = compressor.compress(test_str);

    // 解压数据
    std::string decompressed = compressor.decompress(compressed);

    // 检查解压后的数据是否与原始数据相同
    ASSERT_EQ(decompressed, test_str);
}
