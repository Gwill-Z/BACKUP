#include "Packer.h"
#include <sstream>
#include <spdlog/spdlog.h>
#include <c++/10/iomanip>

std::string Packer::pack(std::vector<std::pair<std::string, std::string>> files) {
    std::ostringstream packedStream;
    for (const auto& file : files) {
        // 写入文件路径长度和路径
        std::string pathLengthStr = std::to_string(file.first.size());
        packedStream << std::setw(10) << std::setfill('0') << pathLengthStr << file.first;

        // 写入文件内容长度和内容
        std::string contentLengthStr = std::to_string(file.second.size());
        packedStream << std::setw(10) << std::setfill('0') << contentLengthStr << file.second;
    }
    return packedStream.str();
}




std::vector<std::pair<std::string, std::string>> Packer::unpack(std::string packedData) {
    std::vector<std::pair<std::string, std::string>> files;
    std::istringstream packedStream(packedData);

    while (packedStream) {
        // 读取文件路径长度
        char pathLengthStr[11];
        packedStream.read(pathLengthStr, 10);
        pathLengthStr[10] = '\0'; // 确保字符串以空字符结尾
        size_t pathLength = std::stoul(pathLengthStr);

        // 读取文件路径
        std::string path(pathLength, '\0');
        packedStream.read(&path[0], pathLength);

        // 读取文件内容长度
        char contentLengthStr[11];
        packedStream.read(contentLengthStr, 10);
        contentLengthStr[10] = '\0';
        size_t contentLength = std::stoul(contentLengthStr);

        // 读取文件内容
        std::string content(contentLength, '\0');
        packedStream.read(&content[0], contentLength);

        if (packedStream) {
            files.push_back({path, content});
        }
    }
    return files;
}




