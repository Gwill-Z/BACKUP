#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <vector>

bool isFileSizeInRange(const std::string& filePath, long long minSize, long long maxSize) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0) {
        return false; // 文件不存在或无法访问
    }
    long long fileSize = static_cast<long long>(fileStat.st_size);
    return fileSize >= minSize && fileSize <= maxSize;
}

bool isFileModifiedTimeInRange(const std::string& filePath, std::time_t startTime, std::time_t endTime) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0) {
        return false; // 文件不存在或无法访问
    }
    std::time_t modifiedTime = fileStat.st_mtime;
    return modifiedTime >= startTime && modifiedTime <= endTime;
}

bool isFileInList(const std::string& filePath, const std::vector<std::string>& fileList) {
    for (const auto& file : fileList) {
        if (filePath == file) {
            return true;
        }
    }
    return false;
}

bool isFileInDirectories(const std::string& filePath, const std::vector<std::string>& directoryList) {
    for (const auto& dir : directoryList) {
        if (filePath.find(dir) == 0) { // 检查filePath是否以dir开头
            return true;
        }
    }
    return false;
}

#endif // UTILS_H
