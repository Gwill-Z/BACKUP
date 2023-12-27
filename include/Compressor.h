#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "HuffmanNode.h"
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

class Compressor {
public:
    struct Compare {
        bool operator()(const std::shared_ptr<HuffmanNode>& lhs, const std::shared_ptr<HuffmanNode>& rhs) {
            return lhs->frequency > rhs->frequency;
        }
    };

    std::shared_ptr<HuffmanNode> root;
    std::unordered_map<char, std::string> codes;

    void buildTree(const std::unordered_map<char, int>& frequencies);
    void generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code);
    

public:
    std::string compress(const std::string& data);
    std::string decompress(const std::string& encodedText);
    std::string serializeTree(const std::shared_ptr<HuffmanNode>& node) const;
    std::shared_ptr<HuffmanNode> deserializeTree(const std::string& str, size_t& pos) const;
};

#endif // COMPRESSOR_H
