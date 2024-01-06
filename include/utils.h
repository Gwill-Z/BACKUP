#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <string>

// 函数用于获取文件大小
long getFileSize(const std::string& filePath) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) == -1) {
        perror("stat");
        return -1; // 返回-1表示错误
    }
    return fileStat.st_size; // 返回文件大小
}

// 函数用于获取文件的最后修改时间
std::string getFileLastModifiedTime(const std::string& filePath) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) == -1) {
        perror("stat");
        return ""; // 返回空字符串表示错误
    }

    char timeString[100];
    const struct tm* timeinfo;
    timeinfo = localtime(&fileStat.st_mtime);
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(timeString); // 返回最后修改时间的字符串表示
}