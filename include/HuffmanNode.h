#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <memory>

class HuffmanNode {
public:
    char character;
    int frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
    HuffmanNode(char ch, int freq, std::shared_ptr<HuffmanNode> leftNode, std::shared_ptr<HuffmanNode> rightNode)
        : character(ch), frequency(freq), left(leftNode), right(rightNode) {}
};


#endif // HUFFMAN_NODE_H
