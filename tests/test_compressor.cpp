#include "Compressor.h"
#include "HuffmanNode.h"
#include <gtest/gtest.h>
#include <string>
#include <stdexcept>
#include <bitset>

class CompressorTest : public ::testing::Test {
protected:
    Compressor compressor;
};

// 测试压缩和解压缩
TEST_F(CompressorTest, CompressDecompress) {
    std::string originalText = "This is a test string for Huffman compression.";
    std::string compressedText = compressor.compress(originalText);
    std::string decompressedText = compressor.decompress(compressedText);

    EXPECT_EQ(decompressedText, originalText);
}

// 测试空字符串的压缩和解压缩
TEST_F(CompressorTest, CompressDecompressEmptyString) {
    std::string originalText = "";
    std::string compressedText = compressor.compress(originalText);
    std::string decompressedText = compressor.decompress(compressedText);

    EXPECT_EQ(decompressedText, originalText);
}

// 测试单个字符的压缩和解压缩
TEST_F(CompressorTest, CompressDecompressSingleCharacter) {
    std::string originalText = "a";
    std::string compressedText = compressor.compress(originalText);
    std::string decompressedText = compressor.decompress(compressedText);

    EXPECT_EQ(decompressedText, originalText);
}


