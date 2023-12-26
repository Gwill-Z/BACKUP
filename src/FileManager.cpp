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

std::tuple<std::string, uint32_t> FileManager::readBackupFile(const std::string& backupFilePath) {
    std::ifstream input(backupFilePath, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Unable to open backup file for reading: " + backupFilePath);
    }

    // 移动到文件末尾以读取CRC
    input.seekg(-static_cast<int>(sizeof(uint32_t)), std::ios::end);
    uint32_t crc;
    input.read(reinterpret_cast<char*>(&crc), sizeof(crc));

    // 计算数据部分的大小
    input.seekg(0, std::ios::beg);
    std::streampos startPos = input.tellg();
    input.seekg(-static_cast<int>(sizeof(uint32_t)), std::ios::end);
    std::streampos endPos = input.tellg();
    size_t dataSize = static_cast<size_t>(endPos - startPos);

    // 读取数据部分
    input.seekg(0, std::ios::beg);
    std::string data(dataSize, '\0');
    input.read(&data[0], dataSize);

    return std::make_tuple(data, crc);
}

void FileManager::writeBackupFile(const std::string& backupFilePath, const std::string& data, uint32_t crc) {
    std::ofstream output(backupFilePath, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Unable to open backup file for writing: " + backupFilePath);
    }

    // 写入加密数据
    output.write(data.data(), data.size());

    // 写入CRC值
    output.write(reinterpret_cast<const char*>(&crc), sizeof(crc));
}

void FileManager::createDirectory(const std::string& directoryPath) {
    fs::create_directories(directoryPath);
}

void FileManager::restoreFiles(const std::vector<std::pair<std::string, std::string>>& files, const std::string& targetPath) {
    for (const auto& file : files) {
        // 构建完整的目标文件路径
        std::filesystem::path fullPath = std::filesystem::path(targetPath) / file.first;

        // 创建文件所在目录（如果不存在）
        std::filesystem::create_directories(fullPath.parent_path());

        // 写入文件内容
        std::ofstream output(fullPath, std::ios::binary);
        if (!output) {
            throw std::runtime_error("Unable to open file for writing: " + fullPath.string());
        }
        output.write(file.second.data(), file.second.size());
    }
}
