#include "FileManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

bool FileManager::isDirectory(const std::string& path) {
    return fs::is_directory(fs::status(path));
}

std::string FileManager::readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void FileManager::readDirectory(const std::string& directoryPath, std::vector<std::pair<std::string, std::string>>& files) {
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (!fs::is_directory(entry.status())) {
            files.push_back({entry.path().string(), readFile(entry.path().string())});
        }
    }
}

std::vector<std::pair<std::string, std::string>> FileManager::readPath(const std::string& path) {
    std::vector<std::pair<std::string, std::string>> files;
    if (isDirectory(path)) {
        readDirectory(path, files);
    } else {
        files.push_back({path, readFile(path)});
    }
    return files;
}

void FileManager::writeFile(const std::string& backupFilePath, std::vector<std::pair<std::string, std::string>>& files) {
    std::ofstream output(backupFilePath, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Unable to open backup file for writing: " + backupFilePath);
    }

    for (const auto& file : files) {
        // 记录文件路径长度和路径
        size_t pathLength = file.first.size();
        output.write(reinterpret_cast<const char*>(&pathLength), sizeof(pathLength));
        output.write(file.first.c_str(), pathLength);

        // 记录文件内容长度和内容
        size_t contentLength = file.second.size();
        output.write(reinterpret_cast<const char*>(&contentLength), sizeof(contentLength));
        output.write(file.second.data(), contentLength);
    }
}

void FileManager::createDirectory(const std::string& directoryPath) {
    fs::create_directories(directoryPath);
}

void FileManager::restoreFiles(const std::string& backupFilePath, const std::string& targetPath) {
    std::ifstream input(backupFilePath, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Unable to open backup file for reading: " + backupFilePath);
    }

    while (!input.eof()) {
        // 读取文件路径长度和路径
        size_t pathLength;
        input.read(reinterpret_cast<char*>(&pathLength), sizeof(pathLength));
        std::string filePath(pathLength, '\0');
        input.read(&filePath[0], pathLength);

        // 读取文件内容长度和内容
        size_t contentLength;
        input.read(reinterpret_cast<char*>(&contentLength), sizeof(contentLength));
        std::vector<char> content(contentLength);
        input.read(content.data(), contentLength);

        // 检查是否已到达文件末尾
        if (input.eof()) break;

        // 创建文件所在目录
        fs::path fullTargetPath = fs::path(targetPath) / filePath;
        fs::create_directories(fullTargetPath.parent_path());

        // 写入文件内容
        std::ofstream output(fullTargetPath, std::ios::binary);
        if (!output) {
            throw std::runtime_error("Unable to open file for writing: " + fullTargetPath.string());
        }
        output.write(content.data(), content.size());
    }
}
