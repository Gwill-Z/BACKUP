#include "Packer.h"
#include <sstream>

std::string Packer::pack(std::vector<std::pair<std::string, std::string>> files) {
    std::ostringstream packedStream;
    for (const auto& file : files) {
        // 写入文件路径长度和路径
        size_t pathLength = file.first.size();
        packedStream.write(reinterpret_cast<const char*>(&pathLength), sizeof(pathLength));
        packedStream << file.first;

        // 写入文件内容长度和内容
        size_t contentLength = file.second.size();
        packedStream.write(reinterpret_cast<const char*>(&contentLength), sizeof(contentLength));
        packedStream << file.second;
    }
    return packedStream.str();
}

std::vector<std::pair<std::string, std::string>> Packer::unpack(std::string packedData) {
    std::vector<std::pair<std::string, std::string>> files;
    std::istringstream packedStream(packedData);
    while (packedStream) {
        // 读取文件路径长度和路径
        size_t pathLength;
        packedStream.read(reinterpret_cast<char*>(&pathLength), sizeof(pathLength));
        std::string path(pathLength, '\0');
        packedStream.read(&path[0], pathLength);

        // 读取文件内容长度和内容
        size_t contentLength;
        packedStream.read(reinterpret_cast<char*>(&contentLength), sizeof(contentLength));
        std::string content(contentLength, '\0');
        packedStream.read(&content[0], contentLength);

        if (packedStream) {
            files.push_back({path, content});
        }
    }
    return files;
}
