#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include <vector>

class Compressor {
public:
    std::vector<std::pair<std::string, std::string>> compress(std::vector<std::pair<std::string, std::string>>& data);
    std::vector<std::pair<std::string, std::string>> decompress(std::vector<std::pair<std::string, std::string>>& data);

private:
    // 可以添加一些私有方法或成员变量以支持压缩和解压缩
};

#endif // COMPRESSOR_H
