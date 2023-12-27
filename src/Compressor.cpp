#include "Compressor.h"
#include <bitset>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <iostream>
#include <cstdio>  

void printHuffmanTree(const std::shared_ptr<HuffmanNode>& node, std::string prefix) {
    if (!node) {
        return;
    }

    // 打印当前节点
    if (node->character != '\0') {
        std::cout << prefix << "'" << node->character << "' (" << node->frequency << ")\n";
    } else {
        std::cout << prefix << "Internal (" << node->frequency << ")\n";
    }

    // 递归打印左子树和右子树
    printHuffmanTree(node->left, prefix + "  L--");
    printHuffmanTree(node->right, prefix + "  R--");
}

void Compressor::buildTree(const std::unordered_map<char, int>& frequencies) {
    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, Compare> minHeap;

    for (const auto& pair : frequencies) {
        minHeap.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        auto left = minHeap.top(); minHeap.pop();
        auto right = minHeap.top(); minHeap.pop();
        auto sum = left->frequency + right->frequency;
        auto node = std::make_shared<HuffmanNode>('\0', sum);
        node->left = left;
        node->right = right;
        minHeap.push(node);
    }

    root = minHeap.top();
}

void Compressor::generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code) {
    if (!node) return;
    if (node->character != '\0') {
        codes[node->character] = code;
    }
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::string Compressor::serializeTree(const std::shared_ptr<HuffmanNode>& node) const {
    if (!node) {
        return "";
    }
    if (node->character != '\0') {
        return "1" + std::string(1, node->character);
    }
    return "0" + serializeTree(node->left) + serializeTree(node->right);
}

std::shared_ptr<HuffmanNode> Compressor::deserializeTree(const std::string& str, size_t& pos) const {
    if (pos >= str.size()) {
        return nullptr;
    }
    if (str[pos] == '1') {
        pos++;
        return std::make_shared<HuffmanNode>(str[pos++], 0);
    }
    pos++;
    auto left = deserializeTree(str, pos);
    auto right = deserializeTree(str, pos);
    return std::make_shared<HuffmanNode>('\0', 0, left, right);
}

std::string Compressor::compress(const std::string& data) {
    // Step 1: Calculate frequencies
    std::unordered_map<char, int> frequencies;
    for (char ch : data) {
        frequencies[ch]++;
    }

    // Step 2: Build Huffman Tree
    buildTree(frequencies);

    // Step 3: Generate Huffman codes
    generateCodes(root, "");


    // 将编码转换为比特串
    std::vector<bool> bitStream;
    for (char ch : data) {
        std::string code = codes[ch];
        for (char bit : code) {
            bitStream.push_back(bit == '1');
        }
    }

    // 字节填充
    size_t paddingLength = 8 - (bitStream.size() % 8);
    for (size_t i = 0; i < paddingLength; ++i) {
        bitStream.push_back(false);
    }

    // 转换比特串为字节
    std::string encoded;
    for (size_t i = 0; i < bitStream.size(); i += 8) {
        char byte = 0;
        for (size_t bit = 0; bit < 8; ++bit) {
            byte |= bitStream[i + bit] << (7 - bit);
        }
        encoded.push_back(byte);
    }

    // 序列化树和组合填充信息、编码文本
    std::string serializedTree = serializeTree(root);
    std::string paddingInfo = std::to_string(paddingLength);
    return serializedTree + '\x02' + paddingInfo + '\x02' + encoded;
    
}


std::string Compressor::decompress(const std::string& encodedText) {
    // 分离序列化的树、填充信息和编码文本
    size_t firstDelimiterPos = encodedText.find('\x02');
    size_t secondDelimiterPos = encodedText.find('\x02', firstDelimiterPos + 1);
    std::string serializedTree = encodedText.substr(0, firstDelimiterPos);
    size_t paddingLength = std::stoi(encodedText.substr(firstDelimiterPos + 1, secondDelimiterPos - firstDelimiterPos - 1));
    std::string encoded = encodedText.substr(secondDelimiterPos + 1);

    // 反序列化树
    size_t pos = 0;
    root = deserializeTree(serializedTree, pos);


    // 转换字节为比特串
    std::vector<bool> bitStream;
    for (char byte : encoded) {
        for (int bit = 0; bit < 8; ++bit) {
            bitStream.push_back(byte & (1 << (7 - bit)));
        }
    }

    // 去除填充
    bitStream.erase(bitStream.end() - paddingLength, bitStream.end());
    // 解码
    std::string decodedText;
    std::shared_ptr<HuffmanNode> currentNode = root;

    for (bool bit : bitStream) {
        currentNode = bit ? currentNode->right : currentNode->left;
        if (!currentNode) {
            throw std::runtime_error("Decoding error: Invalid bitstream or tree structure.");
            // break;
        }

        if (currentNode->character != '\0') {
            decodedText += currentNode->character;
            currentNode = root;
        }
    }

    return decodedText;
}


